#include "renderer.h"

Renderer::Renderer(string _gamePackage, EntityManager* _manager)
  : gamePackage(_gamePackage), manager(_manager) {
  this->running = true;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL: " << SDL_GetError() << std::endl;
    SDL_Quit();
    throw renderer_error();
  }

  // creating a window
  this->win = SDL_CreateWindow(
    "Game",
    0, 0,
    this->windowWidth, this->windowHeight,
    SDL_WINDOW_SHOWN
  );
  if (this->win == nullptr) {
    std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    throw renderer_error();
  }

  // creating a renderer
  this->ren = SDL_CreateRenderer(
    this->win,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  if (this->ren == nullptr) {
    SDL_DestroyWindow(this->win);
    std::cout << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    throw renderer_error();
  }

  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
    std::cout << "IMG_Init Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    throw renderer_error();
  }

  //Initialize SDL_ttf
  if(TTF_Init() == -1) {
    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
    SDL_Quit();
    throw renderer_error();
  }

  SDL_SetRenderDrawBlendMode(this->ren, SDL_BLENDMODE_BLEND);

  string gameFile = gamePackage + "/app.json";
  cout << gameFile << endl;
  json game_data = readFile(gameFile.c_str());
  json maps = game_data.at("maps");

  for (auto& element : json::iterator_wrapper(maps)) {
    auto map = element.value();

    string name = map.at("name").get<string>();
    string id = map.at("id").get<string>();
    mapsByName[name] = id;
  }

  this->grid.tile_w = game_data.at("tile").at("width").get<int>();
  this->grid.tile_h = game_data.at("tile").at("height").get<int>();

  int mapIndex = game_data.at("initialMap").get<int>();
  json currentMap = game_data.at("maps").at(mapIndex);
  string levelId = currentMap.at("id").get<string>();

  Entity* camera = manager->createEntity();
  manager->addComponent<DimensionComponent>(camera, this->windowWidth, this->windowHeight);
  manager->addComponent<PositionComponent>(camera, 0, 0);
  manager->saveSpecial("camera", camera);

  textures["flame"] = loadTexture(this->ren, "assets/flame.png");

  auto tileset_data = game_data.at("tilesets");
  for (uint i = 0; i < tileset_data.size(); i++) {
    auto tileset = tileset_data.at(i);

    int rows = tileset.at("rows").get<int>();
    int columns = tileset.at("columns").get<int>();
    string type = tileset.at("type").get<string>();
    string src = "assets/" + tileset.at("src").get<string>();
    auto tr = tileset.at("terrains");
    map<int, string> terrains;

    for (auto& element : json::iterator_wrapper(tr)) {
      int key = stoi(element.key());
      string value = element.value().at("type").get<string>();
      terrains[key] = value;
    }

    SDL_Texture *texture = loadTexture(this->ren, src.c_str());

    Tileset *t = new Tileset(rows, columns, type, texture, terrains);

    this->tilesets.push_back(t);
  }

  auto anims = game_data.at("animations");
  for (auto &i : json::iterator_wrapper(anims)) {
    string key = i.key();
    auto animation = i.value();

    Animation anim = Animation();

    anim.id = animation.at("id").get<string>();
    anim.numberOfFrames = animation.at("numberOfFrames").get<int>();

    auto keyframes = animation.at("keyframes");
    for (auto &j : json::iterator_wrapper(keyframes)) {
      int key = stoi(j.key());
      json value = j.value();

      SDL_Rect r;
      r.x = value.at("x").get<int>();
      r.y = value.at("y").get<int>();
      r.w = value.at("w").get<int>();
      r.h = value.at("h").get<int>();

      anim.keyframes[key] = r;
    }

    this->animations[key] = anim;
  }

  this->loadStage(levelId);

  this->registerSystem<TransitionSystem>(manager);
  this->registerSystem<InputSystem>(manager);
  this->registerSystem<WalkSystem>(manager);
  this->registerSystem<ProjectileSystem>(manager);
  this->registerSystem<CollisionSystem>(manager);
  this->registerSystem<CameraSystem>(manager);
  this->registerSystem<RenderSystem>(manager);
};

void Renderer::loadStage(string level) {
  string gameFile = gamePackage + "/app.json";
  cout << gameFile << endl;
  json game_data = readFile(gameFile.c_str());

  this->loadStage(game_data, level);
}

void Renderer::loadStage(json game_data, string level) {
  string mapFile = gamePackage + "/maps/" + level + ".json";

  cout << "Loading file: " << mapFile << endl;

  json map_data = readFile(mapFile.c_str());

  this->grid.columns = map_data.at("grid").at("columns").get<int>();
  this->grid.rows = map_data.at("grid").at("rows").get<int>();

  for (uint i = 0; i < map_data.at("layers").size(); i++) {
    json element = map_data.at("layers").at(i);
    json data = element.at("data");

    string type = element.at("type").get<string>();
    string name = element.at("name").get<string>();

    cout << "Loading layer: " << name << endl;

    for (int j = 0; j < data.size(); j++) {
      json node = data.at(j);
      int setIndex = node.at(0).get<int>();

      if (setIndex >= 0) {
        int tileIndex = node.at(1).get<int>();
        Entity* entity = manager->createEntity();
        int surrounding = this->grid.findSurroundings(node, j, data);

        int x = this->grid.getX(j) * this->grid.tile_w;
        int y = this->grid.getY(j) * this->grid.tile_h;

        manager->addComponent<TileComponent>(entity, setIndex, tileIndex, j, surrounding);
        manager->addComponent<PositionComponent>(entity, x, y);
        manager->addComponent<RenderComponent>(entity, i);
      }
      // only continue if it's a action entity
      if (setIndex == -2) {
        string entityId = node.at(1).get<string>();
        json entity_definition = game_data.at("entities").at(entityId);
        json components = entity_definition.at("components");
        string name = entity_definition.at("name").get<string>();

        int x = this->grid.tile_w * this->grid.getX(j);
        int y = this->grid.tile_h * this->grid.getY(j);
        int w = this->grid.tile_w;
        int h = this->grid.tile_h;

        Entity* entity = manager->createEntity();

        if (name == "player") {
          manager->saveSpecial("player", entity);
        }

        for (uint k = 0; k < components.size(); k++) {
          auto component = components.at(k);
          string name = component.at("name").get<string>();

          if (name == "CollisionComponent") {
            bool isStatic = component.at("members").at("isStatic").at("value").get<bool>();
            int x = component.at("members").at("x").at("value").get<int>();
            int y = component.at("members").at("y").at("value").get<int>();
            int w = component.at("members").at("w").at("value").get<int>();
            int h = component.at("members").at("h").at("value").get<int>();

            manager->addComponent<CollisionComponent>(
              entity,
              isStatic,
              0,
              x,
              y,
              (w > 0) ? w : this->grid.tile_w,
              (h > 0) ? h : this->grid.tile_h
            );
          }
          else if (name == "PositionComponent") {
            manager->addComponent<PositionComponent>(entity, x, y);
          }
          else if (name == "DimensionComponent") {
            manager->addComponent<DimensionComponent>(entity, w, h);
          }
          else if (name == "HealthComponent") {
            int hearts = component.at("members").at("hearts").at("value").get<int>();
            int max = component.at("members").at("max").at("value").get<int>();

            manager->addComponent<HealthComponent>(entity, hearts, max);
          }
          else if (name == "SpriteComponent") {
            string source = component.at("members").at("src").at("value").get<string>();
            SDL_Texture *texture = loadTexture(this->ren, source.c_str());

            manager->addComponent<SpriteComponent>(entity, 0, 0, w, h, texture);
          }
          else if (name == "InputComponent") {
            manager->addComponent<InputComponent>(entity);
          }
          else if (name == "RenderComponent") {
            manager->addComponent<RenderComponent>(entity, i);
          }
          else if (name == "MovementComponent") {
            int vecX = component.at("members").at("vecX").at("value").get<int>();
            int vecY = component.at("members").at("vecY").at("value").get<int>();

            manager->addComponent<MovementComponent>(entity, vecX, vecY);
          }
          else if (name == "WalkComponent") {
            manager->addComponent<WalkComponent>(entity);
          }
          else if (name == "CenteredCameraComponent") {
            EID camera = manager->getSpecial("camera");
            manager->addComponent<CenteredCameraComponent>(camera, entity->eid);
          }
        }
      }
    }
  }
}

void Renderer::loop(float dt) {
  SDL_Event event;

  // extract input information so that all systems can use it
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      this->quit();
    }

    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym)
      {
        case SDLK_UP:
          if (!(Compass::NORTH & compass)) compass += Compass::NORTH;
        break;
        case SDLK_RIGHT:
          if (!(Compass::EAST & compass)) compass += Compass::EAST;
        break;
        case SDLK_DOWN:
          if (!(Compass::SOUTH & compass)) compass += Compass::SOUTH;
        break;
        case SDLK_LEFT:
          if (!(Compass::WEST & compass)) compass += Compass::WEST;
        break;
        case SDLK_SPACE:
          if (!(Actions::MAIN & actions)) actions += Actions::MAIN;
        break;
      }
    }
    if (event.type == SDL_KEYUP) {
      switch (event.key.keysym.sym)
      {
        case SDLK_UP:
          if (Compass::NORTH & compass) compass -= Compass::NORTH;
        break;
        case SDLK_RIGHT:
          if (Compass::EAST & compass) compass -= Compass::EAST;
        break;
        case SDLK_DOWN:
          if (Compass::SOUTH & compass) compass -= Compass::SOUTH;
        break;
        case SDLK_LEFT:
          if (Compass::WEST & compass) compass -= Compass::WEST;
        break;
        case SDLK_SPACE:
          if (Actions::MAIN & actions) actions -= Actions::MAIN;
        break;
      }
    }
  }

  for (int i = 0; i < this->systems.size(); i++) this->systems[i]->update(dt);
}

Renderer::~Renderer() {
  SDL_DestroyRenderer(this->ren);
  SDL_DestroyWindow(this->win);
}


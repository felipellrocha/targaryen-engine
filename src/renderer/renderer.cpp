#include "renderer.h"

Renderer::Renderer(string gamePackage) {
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

#ifdef DRAW_FPS
  //Initialize SDL_ttf
  if(TTF_Init() == -1) {
    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
    SDL_Quit();
    throw renderer_error();
  }
#endif


  string gameFile = gamePackage + "/app.json";
  cout << gameFile << endl;
  json game_data = readFile(gameFile.c_str());

  int mapIndex = game_data.at("initialMap").get<int>();
  auto currentMap = game_data.at("maps").at(mapIndex);
  string mapFile = gamePackage + "/maps/" + currentMap.at("id").get<string>() + ".json";

  entities = game_data.at("entities");
  //animations = game_data.at("animations");

  cout << "Loading file: " << mapFile << endl;

  json map_data = readFile(mapFile.c_str());

  this->grid.columns = map_data.at("grid").at("columns").get<int>();
  this->grid.rows = map_data.at("grid").at("rows").get<int>();
  this->grid.tile_w = game_data.at("tile").at("width").get<int>();
  this->grid.tile_h = game_data.at("tile").at("height").get<int>();

  EntityManager *manager = new EntityManager();

  Entity* camera = manager->createEntity();
  manager->addComponent<DimensionComponent>(camera, this->windowWidth, this->windowHeight);
  manager->addComponent<PositionComponent>(camera, 0, 0);
  manager->saveSpecial("camera", camera);

  textures["flame"] = IMG_LoadTexture(this->ren, "assets/flame.png");

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

    cout << ": Loading texture: " << src << ", " << type << endl;

    SDL_Texture *texture = IMG_LoadTexture(this->ren, src.c_str());
    if (texture == nullptr){
      std::cout << "LoadTexture Error: " << SDL_GetError() << std::endl;
      IMG_Quit();
      SDL_Quit();
      throw renderer_error();
    }

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

  for (uint i = 0; i < map_data.at("layers").size(); i++) {
    auto element = map_data.at("layers").at(i);
    string type = element.at("type").get<string>();
    string name = element.at("name").get<string>();

    cout << "Loading layer: " << name << endl;

    if (name == "action") {
      auto data = element.at("data");
      for (int j = 0; j < data.size(); j++) {
        auto node = data.at(j);
        int setIndex = node.at(0).get<int>();

        // only continue if it's an entity
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
              SDL_Texture *texture = IMG_LoadTexture(this->ren, source.c_str());
              cout << ": Loading texture: " << source << endl;
              manager->addComponent<SpriteComponent>(entity, 0, 0, w, h, texture);
            }
            else if (name == "InputComponent") {
              manager->addComponent<InputComponent>(entity);
            }
            else if (name == "RenderComponent") {
              manager->addComponent<RenderComponent>(entity);
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
              manager->addComponent<CenteredCameraComponent>(camera, entity->eid);
            }
          }
        }
      }
    }
    else {
      Entity* tile = manager->createEntity();
      auto layer = TileLayer(this->ren, this->tilesets, game_data, map_data, i);
      manager->addComponent<GridComponent>(tile, layer);
      manager->addComponent<RenderComponent>(tile);
    }
  }

  this->registerSystem(new InputSystem(manager, this));
  this->registerSystem(new WalkSystem(manager, this));
  this->registerSystem(new ProjectileSystem(manager, this));
  this->registerSystem(new CollisionSystem(manager, this));
  this->registerSystem(new CameraSystem(manager, this));
  this->registerSystem(new RenderSystem(manager, this));
};

void Renderer::registerSystem(System *system) {
  this->systems.push_back(system);
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


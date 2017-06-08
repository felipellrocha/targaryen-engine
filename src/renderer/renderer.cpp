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

  cout << "Loading file: " << mapFile << endl;

  json map_data = readFile(mapFile.c_str());

  this->grid.columns = map_data.at("grid").at("columns").get<int>();
  this->grid.rows = map_data.at("grid").at("rows").get<int>();
  this->grid.tile_w = game_data.at("tile").at("width").get<int>();
  this->grid.tile_h = game_data.at("tile").at("height").get<int>();

  EntityManager *manager = new EntityManager();

  Entity* camera = manager->createEntity();
  manager->addComponent<DimensionComponent>(camera, new DimensionComponent(this->windowWidth, this->windowHeight));
  manager->addComponent<PositionComponent>(camera, new PositionComponent(0, 0));
  manager->saveCamera(camera);

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

  for (uint i = 0; i < map_data.at("layers").size(); i++) {
    auto element = map_data.at("layers").at(i);
    string type = element.at("type").get<string>();
    // ugh. awful hack. will fix this shortly...
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
          auto entity_definition = game_data.at("entities").at(entityId);
          auto components = entity_definition.at("components");

          string name = entity_definition.at("name");

          cout << name << endl;

          int x = this->grid.tile_w * this->grid.getX(j);
          int y = this->grid.tile_h * this->grid.getY(j);
          int w = this->grid.tile_w;
          int h = this->grid.tile_h;

          Entity* entity = manager->createEntity();
          for (uint k = 0; k < components.size(); k++) {
            auto component = components.at(k);
            string name = component.at("name").get<string>();

            if (name == "CollisionComponent") {
              cout << name << endl;
              bool isStatic = component.at("members").at("isStatic").at("value").get<bool>();
              manager->addComponent<CollisionComponent>(entity, new CollisionComponent(isStatic));
            }
            if (name == "PositionComponent") {
              cout << name << endl;
              manager->addComponent<PositionComponent>(entity, new PositionComponent(x, y));
            }
            if (name == "DimensionComponent") {
              cout << name << endl;
              manager->addComponent<DimensionComponent>(entity, new DimensionComponent(w, h));
            }
            if (name == "HealthComponent") {
              cout << name << endl;
              int hearts = stoi(component.at("members").at("hearts").at("value").get<string>());
              int max = stoi(component.at("members").at("max").at("value").get<string>());
              manager->addComponent<HealthComponent>(entity, new HealthComponent(hearts, max));
            }
            if (name == "SpriteComponent") {
              cout << name << endl;
              string source = component.at("members").at("src").at("value").get<string>();
              SDL_Texture *texture = IMG_LoadTexture(this->ren, source.c_str());
              cout << ": Loading texture: " << source << endl;
              manager->addComponent<SpriteComponent>(entity, new SpriteComponent(0, 0, w, h, texture));
            }
            if (name == "InputComponent") {
              cout << name << endl;
              manager->addComponent<InputComponent>(entity, new InputComponent());
            }
            if (name == "RenderComponent") {
              cout << name << endl;
              manager->addComponent<RenderComponent>(entity, new RenderComponent());
            }
            if (name == "MovementComponent") {
              cout << name << endl;
              int vecX = stoi(component.at("members").at("vecX").at("value").get<string>());
              int vecY = stoi(component.at("members").at("vecY").at("value").get<string>());
              manager->addComponent<MovementComponent>(entity, new MovementComponent(vecX, vecY));
            }
            if (name == "CenteredCameraComponent") {
              manager->addComponent<CenteredCameraComponent>(camera, new CenteredCameraComponent(entity->eid));
            }
          }
        }
      }
    }
    else {
      Entity* tile = manager->createEntity();
      auto layer = TileLayer(this->ren, this->tilesets, game_data, map_data, i);
      manager->addComponent<GridComponent>(tile, new GridComponent(layer));
      manager->addComponent<RenderComponent>(tile, new RenderComponent());
    }
  }

  this->registerSystem(new InputSystem(manager, this));
  this->registerSystem(new CollisionSystem(manager, this));
  this->registerSystem(new ProjectileSystem(manager, this));
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


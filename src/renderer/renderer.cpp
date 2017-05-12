#include "renderer.h"

Renderer::Renderer(string levelFile) {
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


  json level_data = readFile(levelFile.c_str());

  this->grid.columns = level_data.at("grid").at("columns").get<int>();
  this->grid.rows = level_data.at("grid").at("rows").get<int>();
  this->grid.tile_w = level_data.at("tile").at("width").get<int>();
  this->grid.tile_h = level_data.at("tile").at("height").get<int>();

  EntityManager manager = EntityManager();

  Entity* camera = manager.createEntity();
  manager.addComponent<DimensionComponent>(camera, new DimensionComponent(this->windowWidth, this->windowHeight));
  manager.addComponent<PositionComponent>(camera, new PositionComponent(0, 0));
  manager.saveCamera(camera);


  auto tileset_data = level_data.at("tilesets");
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

  for (uint i = 0; i < level_data.at("layers").size(); i++) {
    auto element = level_data.at("layers").at(i);
    string type = element.at("type").get<string>();
    // ugh. awful hack. will fix this shortly...
    string name = element.at("name").get<string>();

    //if (type == "tile") {
    if (name == "action") {
      // TODO: this all needs to be defined within the editor later
      auto data = element.at("data");
      for (int j = 0; j < data.size(); j++) {
        auto node = data.at(j);
        int setIndex = node.at(0).get<int>();
        int tileIndex = node.at(1).get<int>();

        if (setIndex < 0) continue;

        // collision tile
        if (tileIndex == 0) {
          int x = this->grid.tile_w * this->grid.getX(j);
          int y = this->grid.tile_h * this->grid.getY(j);
          int w = this->grid.tile_w;
          int h = this->grid.tile_h;
          Entity* wall = manager.createEntity();
          manager.addComponent<CollisionComponent>(wall, new CollisionComponent(true));
          manager.addComponent<PositionComponent>(wall, new PositionComponent(x, y));
          manager.addComponent<DimensionComponent>(wall, new DimensionComponent(w, h));
        }
        // character tile
        if (tileIndex == 1) {
          // TODO: Move this out of here
          string source = "assets/character-1.png";
          SDL_Texture *texture = IMG_LoadTexture(this->ren, source.c_str());
          cout << ": Loading texture: " << source << endl;

          if (texture == nullptr){
            cout << "LoadTexture Error: " << SDL_GetError() << endl;
            IMG_Quit();
            SDL_Quit();
            throw;
          }
          // TODO: Move this out of here
          int x = this->grid.tile_w * this->grid.getX(j);
          int y = this->grid.tile_h * this->grid.getY(j);
          int w = this->grid.tile_w;
          int h = this->grid.tile_h;
          Entity* player = manager.createEntity();
          manager.addComponent<CollisionComponent>(player, new CollisionComponent());
          manager.addComponent<HealthComponent>(player, new HealthComponent(5, 5));
          manager.addComponent<MovementComponent>(player, new MovementComponent(4, 4));
          manager.addComponent<PositionComponent>(player, new PositionComponent(x, y));
          manager.addComponent<DimensionComponent>(player, new DimensionComponent(w, h));
          manager.addComponent<SpriteComponent>(player, new SpriteComponent(0, 0, 48, 48, texture));
          manager.addComponent<RenderComponent>(player, new RenderComponent());

          manager.addComponent<CenteredCameraComponent>(camera, new CenteredCameraComponent(player->eid));
        }
      }
    }
    else {
      Entity* tile = manager.createEntity();
      auto layer = TileLayer(this->ren, this->tilesets, level_data, i);
      manager.addComponent<GridComponent>(tile, new GridComponent(layer));
      manager.addComponent<RenderComponent>(tile, new RenderComponent());
    }
  }
  /*
  tree = AABBTree(world.size());

  for (uint i = 0; i < world.size(); i++) tree.insertObject(world[i]);
  */

  // TODO: Move this out of here
  string source = "assets/character-2.png";
  SDL_Texture *texture = IMG_LoadTexture(this->ren, source.c_str());
  cout << ": Loading texture: " << source << endl;

  if (texture == nullptr){
    cout << "LoadTexture Error: " << SDL_GetError() << endl;
    IMG_Quit();
    SDL_Quit();
    throw;
  }
  Entity* player2 = manager.createEntity();
  manager.addComponent<HealthComponent>(player2, new HealthComponent(5, 5));
  manager.addComponent<PositionComponent>(player2, new PositionComponent(48, 0));
  manager.addComponent<SpriteComponent>(player2, new SpriteComponent(0, 0, 48, 48, texture));
  manager.addComponent<RenderComponent>(player2, new RenderComponent());

  this->registerSystem(new InputSystem(manager, this));
  this->registerSystem(new CameraSystem(manager, this));
  this->registerSystem(new RenderSystem(manager, this));
  this->registerSystem(new CollisionSystem(manager, this));

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
      }
    }
  }

  for (int i = 0; i < this->systems.size(); i++) this->systems[i]->update(dt);
}

Renderer::~Renderer() {
  SDL_DestroyRenderer(this->ren);
  SDL_DestroyWindow(this->win);
}


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
    1200, 680,
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
    SDL_RENDERER_ACCELERATED
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

  json level_data = readFile(levelFile.c_str());

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

    if (type == "tile") {
      shared_ptr<TileLayer> layer = make_shared<TileLayer>(this->ren, this->tilesets, level_data, i);
      this->nodes.push_back(layer);
    }
  }
  /*
    if (type == "imagelayer") {
      shared_ptr<ImageLayer> layer = make_shared<ImageLayer>(this->ren, level_data, i);
      this->nodes.push_back(layer);
    }
    if (type == "objectgroup") {
      auto objects = level_data.at("layers").at(i).at("objects");

      for (uint i = 0; i < objects.size(); i++) {
        auto object = objects.at(i);

        shared_ptr<StaticCollisionLayer> layer = make_shared<StaticCollisionLayer>(this->ren, object);
        this->nodes.push_back(layer);
        this->world.push_back(layer);
      }
    }
    }
    if (type == "itemsLayer") {
      auto tiles = level_data.at("layers").at(i).at("data").get<vector<int>>();
      for (uint j = 0; j < tiles.size(); j++) {
        if (tiles[j] > 0) {
          shared_ptr<ItemLayer> node = make_shared<ItemLayer>(
            this->ren,
            level_data,
            j,
            tiles[j],
            i
          );

          this->nodes.push_back(node);
          this->world.push_back(node);
        }
      }
  */

  /*
  // Same as this
  json character_data = readFile("assets/jean.json");

  shared_ptr<CharacterLayer> character = make_shared<CharacterLayer>(this->ren, character_data);

  this->nodes.push_back(character);
  this->world.push_back(character);

  tree = AABBTree(world.size());

  for (uint i = 0; i < world.size(); i++) tree.insertObject(world[i]);
  */
};

Renderer::~Renderer() {
  SDL_DestroyRenderer(this->ren);
  SDL_DestroyWindow(this->win);
}

void Renderer::render() {
  for (uint i = 0; i < this->nodes.size(); i++) this->nodes[i]->render();
}
/*
#ifdef DRAW_AABB
  for (uint i = 0; i < world.size(); i++) {
    auto node = world[i]->getAABB();
  //auto treeNodes = tree.getAllNodes();
  //for (uint i = 0; i < treeNodes.size(); i++) {
  //  auto node = treeNodes[i].aabb;

    SDL_RenderDrawLine(this->ren, node.minX, node.minY, node.maxX, node.minY);
    SDL_RenderDrawLine(this->ren, node.minX, node.maxY, node.maxX, node.maxY);
    SDL_RenderDrawLine(this->ren, node.minX, node.minY, node.minX, node.maxY);
    SDL_RenderDrawLine(this->ren, node.maxX, node.minY, node.maxX, node.maxY);
  }
#endif

*/

void Renderer::update() {
  for (uint i = 0; i < this->nodes.size(); i++) this->nodes[i]->update();
}

void Renderer::physics() {
  for (uint i = 0; i < world.size(); i++) tree.updateObject(world[i]);

  for (uint i = 0; i < world.size(); i++) {
    auto aabb = world[i];
    auto node = dynamic_pointer_cast<Node>(aabb);

    node->collisions.clear();

    auto aabbCollisions = tree.queryOverlaps(aabb);

    for_each(aabbCollisions.begin(), aabbCollisions.end(), [node](const shared_ptr<IAABB>& collidesWith) {
      node->collisions.push_back(dynamic_pointer_cast<Node>(collidesWith));
    });
  };
}

void Renderer::input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      this->quit();
    }

    switch(event.type){
      case SDL_KEYDOWN:
				for (uint i = 0; i < this->nodes.size(); i++) this->nodes[i]->input(event);
        break;

      case SDL_KEYUP:
				for (uint i = 0; i < this->nodes.size(); i++) this->nodes[i]->input(event);
        break;

      default:
        break;
    }
  }
}


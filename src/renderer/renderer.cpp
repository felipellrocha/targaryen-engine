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
    1200, 480,
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

  for (uint i = 0; i < level_data.at("layers").size(); i++) {
    auto element = level_data.at("layers").at(i);
    string type = element
      .at("type")
      .get<string>();

    if (type == "imagelayer") {
      shared_ptr<ImageLayer> layer = make_shared<ImageLayer>(this->ren, level_data, i);
      this->nodes.push_back(layer);
    }
    if (type == "tilelayer") {
      type = element
        .at("properties")
        .at("layerType")
        .get<string>();

      if (type == "itemsLayer") {
        shared_ptr<ItemsLayer> layer = make_shared<ItemsLayer>(
          this->ren,
          &this->world,
          level_data,
          i
        );
        this->nodes.push_back(layer);
      }
      else {
        shared_ptr<TileLayer> layer = make_shared<TileLayer>(this->ren, level_data, i);
        this->nodes.push_back(layer);
      }
    }
  }

  // Same as this
  json character_data = readFile("assets/jean.json");

  shared_ptr<CharacterLayer> character = make_shared<CharacterLayer>(this->ren, character_data);

  this->nodes.push_back(character);
  this->world.push_back(character);

  for (uint i = 0; i < world.size(); i++) tree.insertObject(world[i]);
};

Renderer::~Renderer() {
  SDL_DestroyRenderer(this->ren);
  SDL_DestroyWindow(this->win);
}

void Renderer::render() {

  SDL_RenderClear(this->ren);

  for (uint i = 0; i < this->nodes.size(); i++) this->nodes[i]->render();

#ifdef DRAW_AABB
  //for (uint i = 0; i < world.size(); i++) {
  auto treeNodes = tree.getAllNodes();
  for (uint i = 0; i < treeNodes.size(); i++) {
    auto node = treeNodes[i].aabb;

    SDL_RenderDrawLine(this->ren, node.minX, node.minY, node.maxX, node.minY);
    SDL_RenderDrawLine(this->ren, node.minX, node.maxY, node.maxX, node.maxY);
    SDL_RenderDrawLine(this->ren, node.minX, node.minY, node.minX, node.maxY);
    SDL_RenderDrawLine(this->ren, node.maxX, node.minY, node.maxX, node.maxY);
  }
#endif

  SDL_RenderPresent(this->ren);
}

void Renderer::update() {
  for (uint i = 0; i < this->nodes.size(); i++) this->nodes[i]->update();
}

void Renderer::physics() {
  for (uint i = 0; i < world.size(); i++) tree.updateObject(world[i]);

  for (uint i = 0; i < world.size(); i++) {
    auto sprite = world[i];
    auto aabbCollisions = tree.queryOverlaps(sprite);

    for_each(aabbCollisions.begin(), aabbCollisions.end(), [sprite](const shared_ptr<IAABB>& collidesWith) {
      cout << "Collision!!!" << endl;
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


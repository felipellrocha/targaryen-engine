project('game', 'cpp')
add_global_arguments('-std=c++11', language : 'cpp')

src = [
  'src/json.hpp',

  'src/readjson.h',
  'src/readjson.cpp',

  'src/renderer.h',
  'src/renderer.cpp',

  'src/tile.h',
  'src/tile.cpp',

  'src/main.cpp'
]

sdl = dependency('SDL2')

executable('../game',
  sources : src,
  dependencies : sdl)

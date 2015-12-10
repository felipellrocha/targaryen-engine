project('game', 'cpp')
add_global_arguments('-std=c++11', language : 'cpp')

src = [
  'src/json.hpp',

  'src/timer.h',
  'src/timer.cpp',

  'src/readjson.h',
  'src/readjson.cpp',

  'src/renderer.h',
  'src/renderer.cpp',

  'src/tile.h',
  'src/tile.cpp',

  'src/main.cpp'
]

sdl = dependency('SDL2')
sdl_image = dependency('SDL2_image')

executable('../game',
  sources : src,
  dependencies : [sdl, sdl_image])

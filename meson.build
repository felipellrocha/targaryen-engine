project('game', 'cpp')
add_global_arguments('-std=c++11', language : 'cpp')

src = [
  'src/json.hpp',
  'src/exceptions.h',

  'src/timer.h',
  'src/timer.cpp',

  'src/readjson.h',
  'src/readjson.cpp',

  'src/node.h',

  'src/renderer.h',
  'src/renderer.cpp',

  'src/imagelayer.h',
  'src/imagelayer.cpp',

  'src/tilelayer.h',
  'src/tilelayer.cpp',

  'src/states/basestate.h',

  'src/states/idling.h',
  'src/states/idling.cpp',

  'src/characterlayer.h',
  'src/characterlayer.cpp',

  'src/main.cpp'
]

sdl = dependency('SDL2')
sdl_image = dependency('SDL2_image')

executable('../game',
  sources : src,
  dependencies : [sdl, sdl_image])

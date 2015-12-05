project('game', 'cpp')
add_global_arguments('-std=c++11', language : 'cpp')

src = ['src/tinyxml2.h', 'src/main.cpp']

sdl = dependency('SDL2')

executable('../game', sources : src, dependencies : sdl)

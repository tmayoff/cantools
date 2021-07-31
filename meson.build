project('cantools', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++17'])

imgui_proj = subproject('imgui')
imgui_dep = imgui_proj.get_variable('imgui_dep')

sdl  = dependency('SDL2', required: true)
glew = dependency('glew', required: true)

deps = [imgui_dep, sdl, glew]
exe = executable('cantools', 'src/main.cpp', dependencies: deps, install : true)

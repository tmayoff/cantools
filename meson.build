project('cantools', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++17'])

imgui_proj = subproject('imgui')
imgui_dep = imgui_proj.get_variable('imgui_dep')

sdl  = dependency('SDL2', required: true)
glew = dependency('glew', required: true)

inc = include_directories('src/')
subdir('src')

deps = [imgui_dep, sdl, glew]
exe = executable('cantools', sources, include_directories: inc, dependencies: deps, install : true)

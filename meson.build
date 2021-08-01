project('cantools', 'cpp',
  version : '0.1',
  default_options : ['warning_level=0',
                     'cpp_std=c++17'])

imgui_proj = subproject('imgui')
imgui_dep = imgui_proj.get_variable('imgui_dep')

nlohmann_proj = subproject('nlohmann_json')
nlohmann_dep = nlohmann_proj.get_variable('nlohmann_json_dep')

threads = dependency('threads', required: true)
sdl    = dependency('SDL2', required: true)
glew   = dependency('glew', required: true)

inc = include_directories('src/')
subdir('src')

deps = [threads, imgui_dep, nlohmann_dep, sdl, glew]
exe = executable('cantools', sources, include_directories: inc, dependencies: deps, install : true)

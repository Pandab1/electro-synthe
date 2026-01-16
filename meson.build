project(
  'isen-ap3-electro-synthe',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)
cc=meson.get_compiler('c')
mathlib_dep = cc.find_library('m')

cmake = import('cmake')
raylib_opts = cmake.subproject_options()
raylib_opts.add_cmake_defines({'BUILD_EXAMPLES': 'OFF', 'BUILD_GAMES': 'OFF', 'CMAKE_POLICY_VERSION_MINIMUM': '3.5', 'CMAKE_POSITION_INDEPENDENT_CODE': 'ON'})
raylib_sub = cmake.subproject('raylib', options: raylib_opts)
raylib_dep = raylib_sub.dependency('raylib')

if host_machine.system() == 'darwin'
  raylib_dep = declare_dependency(
    dependencies: [
      raylib_dep,
      dependency('appleframeworks', modules: ['IOKit', 'Cocoa', 'OpenGL', 'CoreVideo'])
    ]
  )
endif

if host_machine.system() == 'linux'
  raylib_dep = declare_dependency(
    dependencies: [
      raylib_dep,
      dependency('glfw3')
    ]
  )
endif

inc = include_directories('include')

subdir('src')

if get_option('build_app')
  subdir('app')
endif

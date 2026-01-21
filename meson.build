project(
  'isen-ap3-electro-synthe',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)
cc=meson.get_compiler('c')
mathlib_dep = cc.find_library('m')

inc = include_directories('include')

subdir('src')

if get_option('build_app')
  subdir('app')
endif

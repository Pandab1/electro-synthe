libbase_project = library('baseproject',
  sources: ['gui.c', 'custom_math.c'],
  include_directories: inc,
  install: true,
  dependencies: [mathlib_dep, raylib_dep]
)

libbase_project_dep = declare_dependency(
  link_with: libbase_project,
  include_directories: inc
)

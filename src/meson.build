libbase_project = library('baseproject',
  sources: ['custom_math.c', 'utils_files.c'],
  include_directories: inc,
  install: true,
  dependencies: [mathlib_dep]
)

libbase_project_dep = declare_dependency(
  link_with: libbase_project,
  include_directories: inc
)

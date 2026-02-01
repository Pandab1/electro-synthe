executable('base_project_app',
  sources: ['main.c'],
  dependencies: [libbase_project_dep, libdsp_project_dep, mathlib_dep, raylib_dep, miniaudio_dep],
  install: true
)

executable('xd',
  sources: ['gui_test.c'],
  include_directories: inc,
  dependencies: [miniaudio_dep, mathlib_dep, raylib_dep, libdsp_project_dep],
  install: true
)

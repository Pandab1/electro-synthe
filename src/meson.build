libbase_project = static_library('baseproject',
  sources: ['gui.c', 'gui_components.c', 'gui_oscillator.c', 'gui_envelop.c','gui_output.c','gui_help.c','custom_math.c','raygui_impl.c', 'utils_files.c'],
  include_directories: inc,
  install: true,
  dependencies: [mathlib_dep, raylib_dep]
)

libbase_project_dep = declare_dependency(
  link_with: libbase_project,
  include_directories: inc
)

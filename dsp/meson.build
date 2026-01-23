libdsp_project = library('dspproject',
  sources: ['adsr.c', 'oscillator.c', 'utils_dsp.c', 'voice.c'],
  include_directories: inc,
  install: true,
  dependencies: [mathlib_dep]
)

libdsp_project_dep = declare_dependency(
  link_with: libdsp_project,
  include_directories: inc
)

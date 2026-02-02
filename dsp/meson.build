libdsp_project = library('dspproject',
  sources: ['dsp_adsr.c', 'dsp_oscillator.c', 'dsp_utils.c', 'dsp_voice.c', 'dsp_api.c'],
  include_directories: inc,
  install: true,
  dependencies: [mathlib_dep, raylib_dep]
)

libdsp_project_dep = declare_dependency(
  link_with: libdsp_project,
  include_directories: inc
)

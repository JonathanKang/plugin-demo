project('plugin-demo', 'c')

pd_dep = [
  dependency('gio-2.0'),
  dependency('glib-2.0'),
  dependency('gmodule-2.0'),
  dependency('gobject-2.0')
]

sources = [
  'pd-main.c',
  'pd-plugin.c',
  'pd-plugin-loader.c'
]

subdir('plugins')

executable(
  'plugin-demo',
  sources,
  dependencies: pd_dep
)

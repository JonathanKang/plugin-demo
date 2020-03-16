project('plugin-demo', 'c')

pd_prefix = get_option('prefix')
pd_bindir = join_paths(pd_prefix, get_option('bindir'))
pd_libdir = join_paths(pd_prefix, get_option('libdir'))

# defines
set_defines = [
  ['LIBDIR', pd_libdir]
]

config_h = configuration_data()
foreach define: set_defines
  config_h.set_quoted(define[0], define[1])
endforeach
configure_file(
  output: 'config.h',
  configuration: config_h
)

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
  install: true,
  install_dir: pd_bindir,
  dependencies: pd_dep
)

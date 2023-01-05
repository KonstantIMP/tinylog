project(
  'tinydns', ['c'],
  version: '1.0.0',
  license: 'GPLv3',
  default_options : [
    'c_std=c18'
  ]
)

install_headers('tinylog.h')

tinylog = shared_library(
  'tinylog',
  ['tinylog.c'],
  include_directories: include_directories('.'),
  install: true
)

pkgc = import('pkgconfig')
pkgc.generate(
  libraries: tinylog,
  version: '1.0',
  name: 'libtinylog',
  filebase: 'tinylog',
  description: 'Tiny library for beautiful (and stupid-easy) logging for c'
)

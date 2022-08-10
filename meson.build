# refer to ~/gtk3/meson.build
project('coldspot', 'c',
  default_options: [
    'prefix=/usr/local',
    'backend=ninja',
    'buildtype=debug',
    'debug=true',
    'strip=false',
    'warning_level=4',
    'werror=true',
    'b_lto=true',
    'b_ndebug=false',
    'c_std=gnu18',
  ],
  license: 'AGPL-3.0',
  meson_version: '>=0.62.0',
  version: '0.1',
)

add_global_arguments('-D_GNU_SOURCE',
                     # '-Wextra',
                     '-Wdeprecated-declarations',
                     '-DG_ENABLE_DEBUG', # /usr/include/gtk-3.0/gtk/gtkdebug.h gtk_set_debug_flags()
                     '-Winline',
                     '-Wshadow',
                     '-Wno-pedantic',
                     '-DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_72', '-DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_72',
                     language: 'c')

executable('coldspot',
  'main.c',
  'nm2.c',
dependencies: [
  dependency('libnm'),
  dependency('libelogind'),
])

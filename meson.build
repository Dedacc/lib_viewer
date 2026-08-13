project('lib_viewer','c')

gtk4 = dependency('gtk4')
libadwaita = dependency('libadwaita-1')

executable(
  'lib_viewer',
  'src/main.c',
  'src/window.c',
  'src/subprocess.c',
  'src/package_detail.c',
  'src/package_detail_dialog.c',
  dependencies: [
    gtk4,
    libadwaita
  ],
)

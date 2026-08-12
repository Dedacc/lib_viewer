project('gpkg-config','c')

gtk4 = dependency('gtk4')
libadwaita = dependency('libadwaita-1')

executable(
  'gpkg-conf',
  'main.c',
  'window.c',
  'subprocess.c',
  'package_detail.c',
  'package_detail_dialog.c',
  dependencies: [
    gtk4,
    libadwaita
  ],
)

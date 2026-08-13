#pragma once

#include <glib-object.h>
#include <adwaita.h>

G_BEGIN_DECLS

#define GPKG_TYPE_OBJECT gpkg_window_get_type()
G_DECLARE_FINAL_TYPE (GpkgWindow, gpkg_window, GPKGWINDOW, Object, AdwApplicationWindow)

GpkgWindow *gpkg_window_new (void);


G_END_DECLS

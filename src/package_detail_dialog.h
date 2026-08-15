#pragma once

#include <gio/gio.h>
#include <adwaita.h>
#include <gtk/gtk.h>
#include "package_detail.h"

G_BEGIN_DECLS

typedef struct _PackageDetail PackageDetail;
#define PACKAGE_DIALOG_TYPE_OBJECT package_dialog_get_type()

G_DECLARE_FINAL_TYPE (PackageDialog, package_dialog, package, DIALOG, AdwDialog)


PackageDialog *package_dialog_object_new (void);
void package_show_dialog(PackageDialog *dialog, PackageDetail *package, AdwApplicationWindow *parent);


G_END_DECLS


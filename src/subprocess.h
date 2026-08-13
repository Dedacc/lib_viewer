#pragma once

#include <gio/gio.h>


G_BEGIN_DECLS

#define PACKAGE_TYPE_OBJECT package_object_get_type()

G_DECLARE_FINAL_TYPE (PackageObject, package_object, PACKAGE, OBJECT, GObject)


PackageObject *package_object_new (void);
void package_object_set_name(PackageObject *self, const char *name, size_t size);
char *package_object_get_name(PackageObject *self);
size_t package_object_get_n_size(PackageObject *self);
void package_object_set_detail(PackageObject *self, const char *detail, size_t size);
char *package_object_get_detail(PackageObject *self);
size_t package_object_get_d_size(PackageObject *self);


GListStore *init_process(void);

G_END_DECLS


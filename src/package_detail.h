#pragma once

#include "window.h"
#include "package_detail_dialog.h"

typedef struct _PackageDetail{
    gchar *name;
    guint n_name;

    gchar *about;
    guint n_about;

    gchar *cflags;
    guint n_cflags;

    gchar *print_requires;
    guint n_requires;

    gchar *libs;
    guint n_libs;
} PackageDetail;

void package_detail_show(const gchar *name, AdwApplicationWindow *parent);


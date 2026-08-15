#include <gio/gio.h>
#include <glib.h>
#include "glibconfig.h"
#include "package_detail.h"

static void package_detail_process(PackageDetail *package_detail,const gchar *option,const gchar *name){
    GSubprocess *process;
    GBytes *buffer;
    gsize size;
    GError *error = NULL;

    package_detail->name = strdup(name);
    package_detail->n_name = strlen(name);

    process = g_subprocess_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE,
			       &error,
			       "pkg-config",
			       option,
			       name,
			       NULL);

    if (process == NULL) {
        g_printerr("FAILED ON THE INIT PART: %s\n", error->message);
        g_error_free(error);
        return ;
    }

    if(g_strcmp0(option, "--about")==0){

	g_subprocess_communicate(process,
				 NULL,
				 NULL,
				 &buffer,
				 NULL,
				 &error);
	
	if (error != NULL) {
	    g_printerr("Failed at the buffer part: %s\n",error->message);
	    g_error_free(error);
	    g_object_unref(process);
	    return ;
	}

	size = g_bytes_get_size(buffer);

	package_detail->about = g_strndup(g_bytes_get_data(buffer, NULL), size);
	package_detail->n_about = size;
	g_bytes_unref(buffer);
    }

    if(g_strcmp0(option, "--cflags")==0){

	g_subprocess_communicate(process,
				 NULL,
				 NULL,
				 &buffer,
				 NULL,
				 &error);

        if (error != NULL) {
          g_printerr("Failed at the buffer part: %s\n", error->message);
          g_error_free(error);
          g_object_unref(process);
          return;
        }
	size = g_bytes_get_size(buffer);

	package_detail->cflags = g_strndup(g_bytes_get_data(buffer, NULL), size);
	package_detail->n_cflags = size;
	g_bytes_unref(buffer);

    }

    if(g_strcmp0(option, "--print-requires")==0){

	g_subprocess_communicate(process,
				 NULL,
				 NULL,
				 &buffer,
				 NULL,
				 &error);

        if (error != NULL) {
          g_printerr("Failed at the buffer part: %s\n", error->message);
          g_error_free(error);
          g_object_unref(process);
          return;
        }

        size = g_bytes_get_size(buffer);

        package_detail->print_requires = g_strndup(g_bytes_get_data(buffer, NULL), size);
        package_detail->n_requires = size;
        g_bytes_unref(buffer);
    }

    if(g_strcmp0(option, "--libs")==0){

	g_subprocess_communicate(process,
				 NULL,
				 NULL,
				 &buffer,
				 NULL,
				 &error);

        if (error != NULL) {
          g_printerr("Failed at the buffer part: %s\n", error->message);
          g_error_free(error);
          g_object_unref(process);
          return;
        }

        size = g_bytes_get_size(buffer);

        package_detail->libs = g_strndup(g_bytes_get_data(buffer, NULL), size);
        package_detail->n_libs = size;
        g_bytes_unref(buffer);
    }
}

void package_detail_show (const gchar *name, AdwApplicationWindow *parent){

    PackageDetail *package_detail;
    PackageDialog *dialog;
    package_detail = g_new0(PackageDetail, 1);
    dialog = g_object_new(PACKAGE_DIALOG_TYPE_OBJECT, NULL);

    package_detail_process(package_detail,"--about", name);
    package_detail_process(package_detail,"--print-requires",name);
    package_detail_process(package_detail,"--cflags",name);
    package_detail_process(package_detail,"--print-requires", name);
    package_detail_process(package_detail,"--libs", name);


    package_show_dialog(dialog,package_detail,parent);
}


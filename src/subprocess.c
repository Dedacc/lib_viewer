#include <gio/gio.h>
#include <glib.h>
#include "subprocess.h"
#include "glibconfig.h"

struct _PackageObject
{
    GObject parent_instance;

    char *name;
    size_t n_size;
    char *detail;
    size_t d_size;
};

G_DEFINE_FINAL_TYPE (PackageObject, package_object, G_TYPE_OBJECT);

void package_object_set_name(PackageObject *self, const char *name, size_t size)
{
    g_return_if_fail (PACKAGE_IS_OBJECT (self));


    // name setter
    self->name = malloc(sizeof(char) * size + 1);
    memcpy(self->name, name, size);
    self->name[size] = '\0';

    // n_size setter
    self->n_size = size;
}

char *package_object_get_name(PackageObject *self){

    g_return_val_if_fail (PACKAGE_IS_OBJECT(self), NULL);

    return self->name;
}

size_t package_object_get_n_size(PackageObject *self){

    g_return_val_if_fail(PACKAGE_IS_OBJECT(self), 0);

    return self->n_size;
}

void package_object_set_detail(PackageObject *self, const char *detail, size_t size){

    g_return_if_fail(PACKAGE_IS_OBJECT(self));

    // detail setter
    self->detail = malloc(sizeof(char) * (size + 1));
    memcpy(self->detail, detail, size);
    self->detail[size] = '\0';

    //d_size setter
    self->d_size = size;
}

char *package_object_get_detail(PackageObject *self){

    g_return_val_if_fail(PACKAGE_IS_OBJECT(self), NULL);

    return self->detail;
}

size_t package_object_get_d_size(PackageObject *self){

    g_return_val_if_fail(PACKAGE_IS_OBJECT(self), 0);

    return self->d_size;
}

static void package_object_class_init(PackageObjectClass *klass){
    
}

static void package_object_init(PackageObject *self){
    
}

GListStore *parser_init(char *data, gsize size, GListStore *store){

    int looper = 0;
    

    while (data[looper] != '\0'){
	int name_start = looper;

	while (data[looper] != ' ') {
	    looper++;
	}

	int name_end = looper;

	int name_length = 0;
	
	name_length = name_end - name_start;

	PackageObject *package = g_object_new(PACKAGE_TYPE_OBJECT, NULL);
	package_object_set_name(package, &data[name_start], name_length);
	while (data[looper] == ' '){
	    looper++;
	}

	int detail_start = looper;

	while (data[looper] != '\n'){
	    looper++;
	}

	int detail_end = looper;
	int detail_length = 0;
	detail_length = detail_end - detail_start;

	package_object_set_detail(package, &data[detail_start], detail_length);

	//next_line
	if(data[looper] == '\n'){
	    looper++;
	}

	g_list_store_append(store, package);
	g_object_unref(package);
    }

    return store;
}

GListStore *init_process(){

    GSubprocess *process;
    GListStore *store;
    GError *error = NULL;
    GBytes *buffer = NULL;
    gsize size = 0;
    gint store_value = 0;
    
    store = g_list_store_new(PACKAGE_TYPE_OBJECT);

    store_value = g_list_model_get_n_items(G_LIST_MODEL(store));
    
    process = g_subprocess_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE,
			       &error,
			       "pkg-config",
			       "--list-all",
			       NULL);

    if (process == NULL) {
        g_printerr("FAILED ON THE INIT PART: %s\n", error->message);
        g_error_free(error);
        return NULL;
    }


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
	return NULL;
    }

    size = g_bytes_get_size(buffer);

    char *data;

    data = g_bytes_get_data(buffer, &size);

    parser_init(data, size, store);
    store_value = g_list_model_get_n_items(G_LIST_MODEL(store));

    g_bytes_unref(buffer);
    g_object_unref(process);

    return store;
}

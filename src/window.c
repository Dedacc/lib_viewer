#include "gio/gio.h"
#include "glib-object.h"
#include "glib.h"
#include "glibconfig.h"
#include <gtk/gtk.h>
#include <adwaita.h>
#include <stdio.h>
#include "window.h"
#include "subprocess.h"
#include "package_detail.h"

struct _GpkgWindow
{
    AdwApplicationWindow parent_instance;

    AdwToolbarView *toolbar;
    AdwHeaderBar *header_bar;
    GtkSearchEntry *search_entry;
    GListStore *store;
    GtkListView *list_view;
    GtkSingleSelection *selection;
    GtkSignalListItemFactory *factory;
    GtkScrolledWindow *scrolled;
    GtkBox *box;
    GtkCustomFilter *filter;
    GtkFilterListModel *filter_model;
};

G_DEFINE_FINAL_TYPE(GpkgWindow, gpkg_window, ADW_TYPE_APPLICATION_WINDOW);

static void
bind_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item){

    GtkBox *box;
    GtkLabel *position;
    AdwActionRow *row;
    PackageObject *package;
    guint number;

    box = GTK_BOX(gtk_list_item_get_child(list_item));
    position = GTK_LABEL(gtk_widget_get_first_child(GTK_WIDGET(box)));
    row = ADW_ACTION_ROW(gtk_widget_get_next_sibling(GTK_WIDGET(position)));
    package = PACKAGE_OBJECT(gtk_list_item_get_item(list_item));
    number = gtk_list_item_get_position(list_item) + 1;

    g_autofree char *text = g_strdup_printf("%u", number);
    gtk_label_set_text(position, text);
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), package_object_get_name(package));
    adw_action_row_set_subtitle(row, package_object_get_detail(package));
}

static gboolean filter_package_cb(gpointer item, gpointer user_data)
{
    PackageObject *package = PACKAGE_OBJECT(item);
    GpkgWindow *self = GPKGWINDOW_Object(user_data);

    const gchar *search = gtk_editable_get_text(GTK_EDITABLE(self->search_entry));
    const gchar *name = package_object_get_name(package);

    if (search[0] == '\0')
        return TRUE;

    return g_str_match_string(search, name, TRUE);
}

static void setup_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item){
    GtkBox *box;
    GtkLabel *position;
    AdwActionRow *row;

    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10));
    position = GTK_LABEL(gtk_label_new(NULL));

    row = ADW_ACTION_ROW(adw_action_row_new());
 
    gtk_box_append(box, GTK_WIDGET(position));
    gtk_box_append(box, GTK_WIDGET(row));

    gtk_list_item_set_child(list_item, GTK_WIDGET(box));
}


static void search_changed_cb(GtkSearchEntry *entry, gpointer user_data){

    GpkgWindow *self = GPKGWINDOW_Object(user_data);

    gtk_filter_changed(GTK_FILTER(self->filter), GTK_FILTER_CHANGE_DIFFERENT);
}

static void list_view_activate_cb(GtkListView *list, guint position, gpointer self){

    PackageObject *package;
    AdwApplicationWindow *window = ADW_APPLICATION_WINDOW(self);
    const gchar *name = NULL;

    package = g_list_model_get_item(G_LIST_MODEL(gtk_list_view_get_model(list)),position);
    name = package_object_get_name(package);
    package_detail_show(name,window);
    
    g_object_unref(package);
    
    
}

static void gpkg_window_class_init(GpkgWindowClass *klass){
}

static void gpkg_window_init(GpkgWindow *self) {

    //init of objects
    self->box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    self->header_bar = ADW_HEADER_BAR (adw_header_bar_new());
    self->toolbar = ADW_TOOLBAR_VIEW(adw_toolbar_view_new());
    self->search_entry = GTK_SEARCH_ENTRY(gtk_search_entry_new());
    self->factory = GTK_SIGNAL_LIST_ITEM_FACTORY(gtk_signal_list_item_factory_new());
    self->scrolled = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new());
    g_signal_connect(GTK_SIGNAL_LIST_ITEM_FACTORY(self->factory), "bind", G_CALLBACK(bind_listitem_cb), NULL);
    g_signal_connect(GTK_SIGNAL_LIST_ITEM_FACTORY(self->factory), "setup", G_CALLBACK(setup_listitem_cb), NULL);
  
  
    // adw_app_window property
    gtk_window_set_title(GTK_WINDOW(self), "lib_viewer");
    gtk_window_maximize(GTK_WINDOW(self));
    gtk_widget_set_size_request(GTK_WIDGET(self), 880, 550);

    //setting child of toolbar and application
    adw_toolbar_view_add_top_bar(self->toolbar, GTK_WIDGET(self->header_bar));
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(self) , GTK_WIDGET(self->toolbar));
    adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW(self->toolbar), GTK_WIDGET(self->box));

    gtk_box_prepend(GTK_BOX(self->box), GTK_WIDGET(self->search_entry));
    self->store = init_process();

    self->filter = gtk_custom_filter_new(filter_package_cb, self, NULL);

    self->filter_model = gtk_filter_list_model_new(G_LIST_MODEL(self->store), GTK_FILTER(self->filter));

    self->selection = GTK_SINGLE_SELECTION(gtk_single_selection_new(G_LIST_MODEL(self->filter_model)));

    self->list_view = GTK_LIST_VIEW(gtk_list_view_new(GTK_SELECTION_MODEL(self->selection), GTK_LIST_ITEM_FACTORY(self->factory)));
    g_signal_connect(GTK_LIST_VIEW(self->list_view), "activate", G_CALLBACK(list_view_activate_cb), self);
    g_signal_connect(GTK_SEARCH_ENTRY(self->search_entry), "search-changed", G_CALLBACK(search_changed_cb), self);

    gtk_widget_set_vexpand(GTK_WIDGET(self->scrolled), TRUE);
    gtk_box_append(GTK_BOX(self->box), GTK_WIDGET(self->scrolled));

    gtk_scrolled_window_set_child(self->scrolled, GTK_WIDGET(self->list_view));

    gtk_widget_set_margin_start(GTK_WIDGET(self->search_entry), 20);
    gtk_widget_set_margin_end(GTK_WIDGET(self->search_entry), 20);
    
    gtk_widget_set_margin_start(GTK_WIDGET(self->list_view), 20);
    gtk_widget_set_margin_end(GTK_WIDGET(self->list_view), 20);

}

#include "package_detail_dialog.h"


struct _PackageDialog{
    AdwDialog parent_instance;

    AdwToolbarView *toolbar;
    AdwHeaderBar *header;
    GtkScrolledWindow *scrolled_window;
    GtkBox *hbox;
    AdwPreferencesGroup *requires;
    AdwActionRow *r_row;
    GtkButton *r_copy;
    AdwPreferencesGroup *cflags;
    AdwActionRow *c_row;
    GtkButton *c_copy;
    AdwPreferencesGroup *libs;
    AdwActionRow *l_row;
    GtkButton *l_copy;
    gchar *s_required;
    gchar *s_cflags;
    gchar *s_libs;
};

G_DEFINE_FINAL_TYPE (PackageDialog, package_dialog, ADW_TYPE_DIALOG);

static void package_dialog_class_init(PackageDialogClass *klass){
    
}

static void copy_cb(GtkButton *button, gpointer user_data){

    PackageDialog *dialog = user_data;
    const gchar *text = NULL;
    GdkClipboard *clipboard;

    if (button == dialog->c_copy)
        text = dialog->s_cflags;

    if (button == dialog->r_copy)
        text = dialog->s_required;

    if (button == dialog->l_copy)
        text = dialog->s_libs;

    if (text == NULL)
        return;

    clipboard = gdk_display_get_clipboard(gdk_display_get_default());

    gdk_clipboard_set_text(clipboard, text);
}

void package_show_dialog(PackageDialog *dialog,PackageDetail *package){

    GtkLabel *title;

    dialog->s_required = g_strdup(package->print_requires);
    dialog->s_cflags = g_strdup(package->cflags);
    dialog->s_libs = g_strdup(package->libs);
    
    title = GTK_LABEL (gtk_label_new(package->name));

    adw_action_row_set_subtitle(ADW_ACTION_ROW(dialog->c_row), "NULL");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(dialog->r_row), "NULL");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(dialog->l_row), "NULL");

    if (package->libs != NULL){
	adw_action_row_set_subtitle(ADW_ACTION_ROW(dialog->l_row), package->libs);
	adw_action_row_set_subtitle_selectable(ADW_ACTION_ROW(dialog->l_row), TRUE);

    }

    if (package->print_requires != NULL){
	adw_action_row_set_subtitle(ADW_ACTION_ROW(dialog->r_row), package->print_requires);
	adw_action_row_set_subtitle_selectable(ADW_ACTION_ROW(dialog->r_row), TRUE);

    }

    if (package->cflags != NULL){
	adw_action_row_set_subtitle(ADW_ACTION_ROW(dialog->c_row), package->cflags);
	adw_action_row_set_subtitle_selectable(ADW_ACTION_ROW(dialog->c_row), TRUE);


    }
    adw_header_bar_set_title_widget(ADW_HEADER_BAR(dialog->header), GTK_WIDGET(title));

    adw_dialog_present(ADW_DIALOG (dialog), NULL);
}

static void
package_dialog_init(PackageDialog *self)
{
    self->toolbar = ADW_TOOLBAR_VIEW(adw_toolbar_view_new());
    self->header = ADW_HEADER_BAR(adw_header_bar_new());
    self->scrolled_window = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new());
    self->hbox = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    self->requires = ADW_PREFERENCES_GROUP(adw_preferences_group_new());
    self->cflags = ADW_PREFERENCES_GROUP(adw_preferences_group_new());
    self->libs = ADW_PREFERENCES_GROUP(adw_preferences_group_new());
    self->r_row = ADW_ACTION_ROW(adw_action_row_new());
    self->c_row = ADW_ACTION_ROW(adw_action_row_new());
    self->l_row = ADW_ACTION_ROW(adw_action_row_new());
    self->c_copy = GTK_BUTTON(gtk_button_new_with_label("Copy"));
    self->r_copy = GTK_BUTTON(gtk_button_new_with_label("Copy"));
    self->l_copy = GTK_BUTTON(gtk_button_new_with_label("Copy"));

    adw_preferences_group_set_title(self->requires, "Required");
    adw_preferences_group_set_title(self->cflags, "Compiler Flags");
    adw_preferences_group_set_title(self->libs, "Libraries");

    gtk_box_append(self->hbox, GTK_WIDGET(self->requires));
    gtk_box_append(self->hbox,GTK_WIDGET(self->cflags));
    gtk_box_append(self->hbox,GTK_WIDGET(self->libs));

    gtk_scrolled_window_set_child(self->scrolled_window,GTK_WIDGET(self->hbox));

    adw_toolbar_view_add_top_bar(self->toolbar, GTK_WIDGET(self->header));
    adw_toolbar_view_set_content(self->toolbar, GTK_WIDGET(self->scrolled_window));
    adw_dialog_set_child(ADW_DIALOG(self), GTK_WIDGET(self->toolbar));

    gtk_widget_set_valign(GTK_WIDGET(self->requires), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(self->cflags), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(self->libs),GTK_ALIGN_CENTER);

    adw_preferences_group_add(ADW_PREFERENCES_GROUP(self->requires), GTK_WIDGET(self->r_row)); 
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(self->libs), GTK_WIDGET(self->l_row));
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(self->cflags), GTK_WIDGET(self->c_row));

    gtk_widget_set_margin_start(GTK_WIDGET(self->requires), 60);
    gtk_widget_set_margin_end(GTK_WIDGET(self->requires), 60);
    gtk_widget_set_margin_start(GTK_WIDGET(self->libs), 60);
    gtk_widget_set_margin_end(GTK_WIDGET(self->libs), 60);
    gtk_widget_set_margin_start(GTK_WIDGET(self->cflags), 60);
    gtk_widget_set_margin_end(GTK_WIDGET(self->cflags), 60);

    gtk_widget_set_valign(GTK_WIDGET(self->l_copy), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(self->r_copy), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(self->c_copy), GTK_ALIGN_CENTER);

    adw_action_row_add_suffix(ADW_ACTION_ROW(self->l_row), GTK_WIDGET(self->l_copy));
    adw_action_row_add_suffix(ADW_ACTION_ROW(self->r_row), GTK_WIDGET(self->r_copy));
    adw_action_row_add_suffix(ADW_ACTION_ROW(self->c_row), GTK_WIDGET(self->c_copy));

    g_signal_connect(self->c_copy, "clicked", G_CALLBACK(copy_cb), self);
    g_signal_connect(self->r_copy, "clicked", G_CALLBACK(copy_cb), self);
    g_signal_connect(self->l_copy, "clicked", G_CALLBACK(copy_cb), self);

    
    adw_dialog_set_content_width(ADW_DIALOG(self),1000);
    adw_dialog_set_content_height(ADW_DIALOG(self),750);
}

#include <gtk/gtk.h>
#include <adwaita.h>
#include <stdio.h>
#include "gio/gio.h"
#include "window.h"
#include "subprocess.h"



static void init(AdwApplication *application){
    GpkgWindow *window;

    window = g_object_new(GPKG_TYPE_OBJECT, "application", application, NULL);

    gtk_window_present(GTK_WINDOW(window));

}





int main (int argc, char **argv){
  g_autoptr (AdwApplication) app = NULL;

  app = adw_application_new ("org.gtk.lib_viewer", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(init), NULL);


  return g_application_run(G_APPLICATION(app), argc, argv);
}

#include <gtk/gtk.h>

int main(int argc, char* argv[])
{
	GtkWidget *window;

	gtk_init(&argc, &argv);
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    grid = gtk_grid_new ();
    gtk_container_add (GTK_CONTAINER (window), grid);
    
    gtk_widget_show_all (window);
    gtk_widget_show (window);
	gtk_main();
	return 0;
}

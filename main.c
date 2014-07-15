#include <gtk/gtk.h>

int main(int argc, char* argv[])
{
	GtkWidget *window,*editor,*viewer, *vbox, *hbox, *button;

	gtk_init(&argc, &argv);
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Swag interpreter");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);

    button = gtk_button_new_with_label("Compile");
    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0); 

    editor = gtk_text_view_new();
    gtk_text_view_set_editable (GTK_TEXT_VIEW (editor), TRUE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (editor), TRUE);
    
    viewer = gtk_text_view_new();
    gtk_text_view_set_editable (GTK_TEXT_VIEW (editor), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (editor), TRUE);

    gtk_box_pack_start(GTK_BOX(hbox), viewer, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), editor, TRUE, TRUE, 5);


    gtk_widget_show_all (window);
	gtk_main();
	return 0;
}

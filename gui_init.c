#include "gui.h"
#include "agenda.h" // Añadido para struct Agenda y MAX_DESC
#include <gtk/gtk.h>

const char *gui_messages[][2] = {
    {"Agenda", "Agenda"}, {"Agregar Evento", "Add Event"}, {"Eliminar Evento", "Delete Event"},
    {"Editar Evento", "Edit Event"}, {"Ver Eventos Próximos", "View Upcoming Events"},
    {"Ver Todos los Eventos", "View All Events"}, {"Cambiar Idioma", "Change Language"},
    {"Salir", "Exit"}, {"Título", "Title"}, {"Descripción", "Description"}, // Sin ":"
    {"Fecha", "Date"}, {"Hora", "Time"}, {"Cancelar", "Cancel"}, {"Guardar", "Save"},
    {"Seleccione un evento para eliminar.", "Select an event to delete."},
    {"Seleccione un evento para editar.", "Select an event to edit."}, {"Error", "Error"},
    {"Formato inválido. Use YYYY-MM-DD y HH:MM.", "Invalid format. Use YYYY-MM-DD and HH:MM."}
};

const char *css = "\
    window { background-color: #f5f5f5; font-family: 'Arial', sans-serif; } \
    button { padding: 8px 16px; border-radius: 4px; background-color: #0078d4; color: white; margin: 5px; } \
    button:hover { background-color: #005ea2; } \
    .treeview { background-color: white; border: 1px solid #ccc; } \
    .treeview row:selected { background-color: #0078d4; color: white; } \
    entry { padding: 8px; border: 1px solid #ccc; border-radius: 4px; margin: 5px; } \
    label { font-weight: bold; margin: 5px; } \
";

void initGUI(int argc, char *argv[], struct Agenda *agenda) {
    gtk_init(&argc, &argv);

    struct GUIData *gui_data = g_malloc(sizeof(struct GUIData));
    if (!gui_data) return;
    gui_data->agenda = agenda;
    gui_data->show_upcoming = 0;
    gui_data->language = 0;

    gui_data->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    if (!gui_data->window) {
        g_free(gui_data);
        return;
    }
    gtk_window_set_title(GTK_WINDOW(gui_data->window), gui_messages[0][gui_data->language]);
    gtk_window_set_default_size(GTK_WINDOW(gui_data->window), 800, 600);
    g_signal_connect(gui_data->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                             GTK_STYLE_PROVIDER(provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(gui_data->window), vbox);

    // Botones principales
    GtkWidget *add_button = gtk_button_new_with_label(gui_messages[1][gui_data->language]);
    GtkWidget *delete_button = gtk_button_new_with_label(gui_messages[2][gui_data->language]);
    GtkWidget *edit_button = gtk_button_new_with_label(gui_messages[3][gui_data->language]);
    GtkWidget *upcoming_button = gtk_button_new_with_label(gui_messages[4][gui_data->language]);
    GtkWidget *all_button = gtk_button_new_with_label(gui_messages[5][gui_data->language]);
    GtkWidget *language_button = gtk_button_new_with_label(gui_messages[6][gui_data->language]);
    GtkWidget *exit_button = gtk_button_new_with_label(gui_messages[7][gui_data->language]);

    gtk_box_pack_start(GTK_BOX(vbox), add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), delete_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), edit_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), upcoming_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), all_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), language_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), exit_button, FALSE, FALSE, 0);

    // Lista de eventos con 5 columnas
    gui_data->list_store = gtk_list_store_new(5, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    if (!gui_data->list_store) {
        gtk_widget_destroy(gui_data->window);
        g_free(gui_data);
        return;
    }
    gui_data->tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gui_data->list_store));
    if (!gui_data->tree_view) {
        g_object_unref(gui_data->list_store);
        gtk_widget_destroy(gui_data->window);
        g_free(gui_data);
        return;
    }

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();

    // Columna "#"
    GtkTreeViewColumn *column0 = gtk_tree_view_column_new_with_attributes("#", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_alignment(column0, 0.5); // Centrado
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui_data->tree_view), column0);
    g_object_set(renderer, "xalign", 0.5, NULL); // Centrar el texto en la columna

    // Columna "Título"
    GtkTreeViewColumn *column1 = gtk_tree_view_column_new_with_attributes(gui_messages[8][gui_data->language], renderer, "text", 1, NULL);
    gtk_tree_view_column_set_alignment(column1, 0.5); // Centrado
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui_data->tree_view), column1);
    g_object_set(renderer, "xalign", 0.5, NULL); // Centrar el texto en la columna

    // Columna "Descripción"
    GtkTreeViewColumn *column2 = gtk_tree_view_column_new_with_attributes(gui_messages[9][gui_data->language], renderer, "text", 2, NULL);
    gtk_tree_view_column_set_alignment(column2, 0.5); // Centrado
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui_data->tree_view), column2);
    g_object_set(renderer, "xalign", 0.5, NULL); // Centrar el texto en la columna

    // Columna "Fecha"
    GtkTreeViewColumn *column3 = gtk_tree_view_column_new_with_attributes(gui_messages[10][gui_data->language], renderer, "text", 3, NULL);
    gtk_tree_view_column_set_alignment(column3, 0.5); // Centrado
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui_data->tree_view), column3);
    g_object_set(renderer, "xalign", 0.5, NULL); // Centrar el texto en la columna

    // Columna "Hora"
    GtkTreeViewColumn *column4 = gtk_tree_view_column_new_with_attributes(gui_messages[11][gui_data->language], renderer, "text", 4, NULL);
    gtk_tree_view_column_set_alignment(column4, 0.5); // Centrado
    gtk_tree_view_append_column(GTK_TREE_VIEW(gui_data->tree_view), column4);
    g_object_set(renderer, "xalign", 0.5, NULL); // Centrar el texto en la columna

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), gui_data->tree_view);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    // Conectar señales
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_event_clicked), gui_data);
    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_event_clicked), gui_data);
    g_signal_connect(edit_button, "clicked", G_CALLBACK(on_edit_event_clicked), gui_data);
    g_signal_connect(upcoming_button, "clicked", G_CALLBACK(on_upcoming_events_clicked), gui_data);
    g_signal_connect(all_button, "clicked", G_CALLBACK(on_all_events_clicked), gui_data);
    g_signal_connect(language_button, "clicked", G_CALLBACK(on_change_language_clicked), gui_data);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_clicked), gui_data);

    updateEventList(gui_data);
    gtk_widget_show_all(gui_data->window);
    gtk_main();
}
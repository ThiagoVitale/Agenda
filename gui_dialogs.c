#include "gui.h"
#include "agenda.h" // Añadido para MAX_DESC
#include <gtk/gtk.h>

GtkWidget *createEventDialog(struct GUIData *gui_data, const char *title, const char *title_text, const char *desc, const char *date, const char *time,
                            GtkWidget **title_entry, GtkWidget **desc_entry, GtkWidget **date_entry, GtkWidget **time_entry) {
    if (!gui_data || !gui_data->window) return NULL;
    GtkWidget *dialog = gtk_dialog_new_with_buttons(title, GTK_WINDOW(gui_data->window), GTK_DIALOG_MODAL,
                                                    gui_messages[12][gui_data->language], GTK_RESPONSE_CANCEL,
                                                    gui_messages[13][gui_data->language], GTK_RESPONSE_OK, NULL);
    if (!dialog) return NULL;

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    GtkWidget *title_label = gtk_label_new(gui_messages[8][gui_data->language]);
    *title_entry = gtk_entry_new();
    if (title_text) gtk_entry_set_text(GTK_ENTRY(*title_entry), title_text);
    gtk_entry_set_max_length(GTK_ENTRY(*title_entry), MAX_DESC - 1);

    GtkWidget *desc_label = gtk_label_new(gui_messages[9][gui_data->language]);
    *desc_entry = gtk_entry_new();
    if (desc) gtk_entry_set_text(GTK_ENTRY(*desc_entry), desc);
    gtk_entry_set_max_length(GTK_ENTRY(*desc_entry), MAX_DESC - 1);

    GtkWidget *date_label = gtk_label_new(gui_messages[10][gui_data->language]);
    *date_entry = gtk_entry_new();
    if (date) gtk_entry_set_text(GTK_ENTRY(*date_entry), date);
    gtk_entry_set_placeholder_text(GTK_ENTRY(*date_entry), "2025-01-01");

    GtkWidget *time_label = gtk_label_new(gui_messages[11][gui_data->language]);
    *time_entry = gtk_entry_new();
    if (time) gtk_entry_set_text(GTK_ENTRY(*time_entry), time);
    gtk_entry_set_placeholder_text(GTK_ENTRY(*time_entry), "00:00");

    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), *title_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), desc_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), *desc_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), date_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), *date_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), time_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), *time_entry, 1, 3, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(content_area);

    return dialog;
}

void showErrorDialog(GtkWidget *parent, const char *message) {
    if (!parent) return;
    struct GUIData *gui_data = g_object_get_data(G_OBJECT(parent), "gui_data");
    if (!gui_data || !gui_data->window) return;
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gui_data->window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", message);
    if (dialog) {
        gtk_window_set_title(GTK_WINDOW(dialog), gui_messages[16][gui_data->language]);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}
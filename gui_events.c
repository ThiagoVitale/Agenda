#include "gui.h"
#include "agenda.h"

void on_add_event_clicked(GtkWidget *widget, struct GUIData *gui_data) {
    if (!gui_data || !gui_data->window) return;
    GtkWidget *title_entry, *desc_entry, *date_entry, *time_entry;
    GtkWidget *dialog = createEventDialog(gui_data, gui_messages[1][gui_data->language], NULL, NULL, NULL, NULL,
                                        &title_entry, &desc_entry, &date_entry, &time_entry);
    if (!dialog) return;

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const char *desc = gtk_entry_get_text(GTK_ENTRY(desc_entry));
        const char *date = gtk_entry_get_text(GTK_ENTRY(date_entry));
        const char *time = gtk_entry_get_text(GTK_ENTRY(time_entry));

        int year, month, day, hour, minute;
        if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3 || sscanf(time, "%d:%d", &hour, &minute) != 2) {
            showErrorDialog(gui_data->window, gui_messages[17][gui_data->language]);
            gtk_widget_destroy(dialog);
            return;
        }

        if (addEvent(gui_data->agenda, title, desc, year, month, day, hour, minute)) {
            updateEventList(gui_data);
        } else {
            showErrorDialog(gui_data->window, gui_messages[0][gui_data->language]);
        }
    }
    gtk_widget_destroy(dialog);
}

void on_delete_event_clicked(GtkWidget *widget, struct GUIData *gui_data) {
    if (!gui_data || !gui_data->window || !gui_data->tree_view) return;
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(gui_data->tree_view));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        showErrorDialog(gui_data->window, gui_messages[14][gui_data->language]);
        return;
    }

    gint index;
    gtk_tree_model_get(model, &iter, 0, &index, -1);
    index--; // Ajustar número a índice
    if (deleteEvent(gui_data->agenda, index)) {
        updateEventList(gui_data);
    } else {
        showErrorDialog(gui_data->window, gui_messages[14][gui_data->language]);
    }
}

void on_edit_event_clicked(GtkWidget *widget, struct GUIData *gui_data) {
    if (!gui_data || !gui_data->window || !gui_data->tree_view) return;
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(gui_data->tree_view));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        showErrorDialog(gui_data->window, gui_messages[15][gui_data->language]);
        return;
    }

    gint index;
    gchar *desc, *date, *time;
    gtk_tree_model_get(model, &iter, 1, &desc, 2, &date, 3, &time, -1); // Índices correctos
    index--; // Ajustar número a índice

    GtkWidget *title_entry, *desc_entry, *date_entry, *time_entry;
    GtkWidget *dialog = createEventDialog(gui_data, gui_messages[3][gui_data->language], NULL, desc, date, time,
                                        &title_entry, &desc_entry, &date_entry, &time_entry);
    if (!dialog) {
        g_free(desc); g_free(date); g_free(time);
        return;
    }

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *new_title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const char *new_desc = gtk_entry_get_text(GTK_ENTRY(desc_entry));
        const char *new_date = gtk_entry_get_text(GTK_ENTRY(date_entry));
        const char *new_time = gtk_entry_get_text(GTK_ENTRY(time_entry));

        int year, month, day, hour, minute;
        if (sscanf(new_date, "%d-%d-%d", &year, &month, &day) != 3 || sscanf(new_time, "%d:%d", &hour, &minute) != 2) {
            showErrorDialog(gui_data->window, gui_messages[17][gui_data->language]);
            g_free(desc); g_free(date); g_free(time);
            gtk_widget_destroy(dialog);
            return;
        }

        if (editEvent(gui_data->agenda, index, new_desc, year, month, day, hour, minute)) {
            updateEventList(gui_data);
        } else {
            showErrorDialog(gui_data->window, gui_messages[14][gui_data->language]);
        }
    }
    g_free(desc); g_free(date); g_free(time);
    gtk_widget_destroy(dialog);
}

void on_upcoming_events_clicked(GtkWidget *widget, struct GUIData *gui_data) {
    if (!gui_data) return;
    gui_data->show_upcoming = 1;
    updateEventList(gui_data);
}

void on_all_events_clicked(GtkWidget *widget, struct GUIData *gui_data) {
    if (!gui_data) return;
    gui_data->show_upcoming = 0;
    updateEventList(gui_data);
}

void on_change_language_clicked(GtkWidget *widget, struct GUIData *gui_data) {
    if (!gui_data || !gui_data->window) return;
    GtkWidget *dialog = gtk_dialog_new_with_buttons(gui_messages[6][gui_data->language], GTK_WINDOW(gui_data->window), GTK_DIALOG_MODAL,
                                                    gui_messages[12][gui_data->language], GTK_RESPONSE_CANCEL,
                                                    gui_messages[13][gui_data->language], GTK_RESPONSE_OK, NULL);
    if (!dialog) return;

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *vbox_lang = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *radio_spanish = gtk_radio_button_new_with_label(NULL, "Español");
    GtkWidget *radio_english = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_spanish), "English");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_spanish), gui_data->language == 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_english), gui_data->language == 1);
    gtk_box_pack_start(GTK_BOX(vbox_lang), radio_spanish, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_lang), radio_english, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(content_area), vbox_lang);
    gtk_widget_show_all(content_area);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        gui_data->language = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_spanish)) ? 0 : 1;
        gtk_window_set_title(GTK_WINDOW(gui_data->window), gui_messages[0][gui_data->language]);
        updateEventList(gui_data);
    }
    gtk_widget_destroy(dialog);
}

void on_exit_clicked(GtkWidget *widget, struct GUIData *gui_data) {
    if (!gui_data) return;
    gtk_main_quit();
}
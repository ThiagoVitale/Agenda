#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

struct GUIData {
    struct Agenda *agenda;
    GtkWidget *window;
    GtkListStore *list_store;
    GtkWidget *tree_view;
    int show_upcoming;
    int language;
};

extern const char *gui_messages[][2];
extern const char *css;

void initGUI(int argc, char *argv[], struct Agenda *agenda);
GtkWidget *createEventDialog(struct GUIData *gui_data, const char *title, const char *title_text, const char *desc, const char *date, const char *time,
                            GtkWidget **title_entry, GtkWidget **desc_entry, GtkWidget **date_entry, GtkWidget **time_entry);
void showErrorDialog(GtkWidget *parent, const char *message);
void updateEventList(struct GUIData *gui_data);
void on_add_event_clicked(GtkWidget *widget, struct GUIData *gui_data);
void on_delete_event_clicked(GtkWidget *widget, struct GUIData *gui_data);
void on_edit_event_clicked(GtkWidget *widget, struct GUIData *gui_data);
void on_upcoming_events_clicked(GtkWidget *widget, struct GUIData *gui_data);
void on_all_events_clicked(GtkWidget *widget, struct GUIData *gui_data);
void on_change_language_clicked(GtkWidget *widget, struct GUIData *gui_data);
void on_exit_clicked(GtkWidget *widget, struct GUIData *gui_data);

#endif
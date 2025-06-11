#include "gui.h"
#include "agenda.h"

void updateEventList(struct GUIData *gui_data) {
    if (!gui_data || !gui_data->list_store) return;
    gtk_list_store_clear(gui_data->list_store);
    int sortedIndices[MAX_EVENTS], activeCount;
    sortEventsByProximity(gui_data->agenda, sortedIndices, &activeCount);

    int displayNumber = 1;
    for (int i = 0; i < activeCount; i++) {
        struct Event *event = &gui_data->agenda->events[sortedIndices[i]];
        if (gui_data->show_upcoming && !isEventUpcoming(*event)) continue;

        char date[11], time[6];
        snprintf(date, sizeof(date), "%04d-%02d-%02d", event->year, event->month, event->day);
        snprintf(time, sizeof(time), "%02d:%02d", event->hour, event->minute);

        GtkTreeIter iter;
        gtk_list_store_append(gui_data->list_store, &iter);
        gtk_list_store_set(gui_data->list_store, &iter,
                          0, displayNumber++, // Número secuencial
                          1, event->title,    // Título
                          2, event->description, // Descripción
                          3, date,           // Fecha
                          4, time,           // Hora
                            -1);
    }
}
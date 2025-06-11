#include <stdio.h>
#include <string.h>
#include <stdarg.h> // Para va_list
#include <time.h>   // Para struct tm
#include "agenda.h"

int language = 0;

// Define the messages array
const char *messages[][2] = {
    {"Error: Agenda llena.", "Error: Agenda is full."},
    {"Error: Descripción vacía.", "Error: Empty description."},
    {"Error: El año debe estar entre 2025 y 9999.", "Error: The year must be between 2025 and 9999."},
    {"Error: El mes debe estar entre 1 y 12.", "Error: The month must be between 1 and 12."},
    {"Error: El día debe ser mayor o igual a 1.", "Error: The day must be greater than or equal to 1."},
    {"Error: El mes %d tiene solo %d días.", "Error: Month %d has only %d days."},
    {"Error: La hora debe estar entre 0 y 23.", "Error: The hour must be between 0 and 23."},
    {"Error: Los minutos deben estar entre 0 y 59.", "Error: Minutes must be between 0 and 59."},
    {"Error: Índice inválido o evento no editable.", "Error: Invalid index or event not editable."},
    {"Evento agregado con éxito.", "Event added successfully."},
    {"Evento actualizado con éxito.", "Event updated successfully."},
    {"Evento eliminado con éxito.", "Event deleted successfully."}
};

void printMessage(int msgIndex, ...) {
    va_list args;
    va_start(args, msgIndex);
    vfprintf(stderr, messages[msgIndex][language], args);
    va_end(args);
}

int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int isValidDate(int year, int month, int day) {
    if (year < 2025 || year > 9999) { printMessage(2); return 0; }
    if (month < 1 || month > 12) { printMessage(3); return 0; }
    if (day < 1) { printMessage(4); return 0; }
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) daysInMonth[1] = 29;
    if (day > daysInMonth[month - 1]) { printMessage(5, month, daysInMonth[month - 1]); return 0; }
    return 1;
}

void initAgenda(struct Agenda *agenda) {
    agenda->count = 0;
    for (int i = 0; i < MAX_EVENTS; i++) agenda->events[i].active = 0;
}

int addEvent(struct Agenda *agenda, const char *title, const char *description, int year, int month, int day, int hour, int minute) {
    if (agenda->count >= MAX_EVENTS) { printMessage(0); return 0; }
    if (strlen(description) == 0) { printMessage(1); return 0; }
    if (!isValidDate(year, month, day)) return 0;
    if (hour < 0 || hour > 23) { printMessage(6); return 0; }
    if (minute < 0 || minute > 59) { printMessage(7); return 0; }

    struct Event newEvent = {0};
    strncpy(newEvent.title, title, MAX_DESC - 1);
    newEvent.title[MAX_DESC - 1] = '\0';
    strncpy(newEvent.description, description, MAX_DESC - 1);
    newEvent.description[MAX_DESC - 1] = '\0';
    newEvent.year = year;
    newEvent.month = month;
    newEvent.day = day;
    newEvent.hour = hour;
    newEvent.minute = minute;
    newEvent.active = 1;

    agenda->events[agenda->count++] = newEvent;
    printMessage(9);
    return 1;
}

int deleteEvent(struct Agenda *agenda, int index) {
    if (index < 0 || index >= agenda->count || !agenda->events[index].active) { printMessage(8); return 0; }
    agenda->events[index].active = 0;
    printMessage(11);
    return 1;
}

int editEvent(struct Agenda *agenda, int index, const char *description, int year, int month, int day, int hour, int minute) {
    if (index < 0 || index >= agenda->count || !agenda->events[index].active) { printMessage(8); return 0; }
    if (strlen(description) == 0) { printMessage(1); return 0; }
    if (!isValidDate(year, month, day)) return 0;
    if (hour < 0 || hour > 23) { printMessage(6); return 0; }
    if (minute < 0 || minute > 59) { printMessage(7); return 0; }

    struct Event *event = &agenda->events[index];
    strncpy(event->title, "", MAX_DESC - 1); // O podrías pasar un título como parámetro si lo deseas
    event->title[MAX_DESC - 1] = '\0';
    strncpy(event->description, description, MAX_DESC - 1);
    event->description[MAX_DESC - 1] = '\0';
    event->year = year; event->month = month; event->day = day;
    event->hour = hour; event->minute = minute;
    printMessage(10);
    return 1;
}

int isEventUpcoming(struct Event event) {
    time_t now = time(NULL);
    struct tm event_time = {0};
    event_time.tm_year = event.year - 1900;
    event_time.tm_mon = event.month - 1;
    event_time.tm_mday = event.day;
    event_time.tm_hour = event.hour;
    event_time.tm_min = event.minute;
    double seconds_diff = difftime(mktime(&event_time), now);
    return seconds_diff >= 0 && seconds_diff <= 14 * 24 * 60 * 60;
}

void sortEventsByProximity(struct Agenda *agenda, int *sortedIndices, int *activeCount) {
    time_t now = time(NULL);
    struct tm event_time = {0};
    *activeCount = 0;
    for (int i = 0; i < agenda->count; i++)
        if (agenda->events[i].active) sortedIndices[(*activeCount)++] = i;
    for (int i = 0; i < *activeCount - 1; i++)
        for (int j = 0; j < *activeCount - i - 1; j++) {
            int idx1 = sortedIndices[j], idx2 = sortedIndices[j + 1];
            event_time.tm_year = agenda->events[idx1].year - 1900;
            event_time.tm_mon = agenda->events[idx1].month - 1;
            event_time.tm_mday = agenda->events[idx1].day;
            event_time.tm_hour = agenda->events[idx1].hour;
            event_time.tm_min = agenda->events[idx1].minute;
            time_t time1 = mktime(&event_time);
            event_time.tm_year = agenda->events[idx2].year - 1900;
            event_time.tm_mon = agenda->events[idx2].month - 1;
            event_time.tm_mday = agenda->events[idx2].day;
            event_time.tm_hour = agenda->events[idx2].hour;
            event_time.tm_min = agenda->events[idx2].minute;
            time_t time2 = mktime(&event_time);
            if (difftime(time1, now) > difftime(time2, now)) {
                int temp = sortedIndices[j];
                sortedIndices[j] = sortedIndices[j + 1];
                sortedIndices[j + 1] = temp;
            }
        }
}
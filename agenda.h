#ifndef AGENDA_H
#define AGENDA_H

#include <stdio.h>  // Para stderr
#include <time.h>   // Para struct tm

#define MAX_EVENTS 100
#define MAX_DESC 100

// Estructura para un evento
struct Event {
    char title[MAX_DESC];
    int year, month, day, hour, minute;
    char description[MAX_DESC];
    int active; // 1 para activo, 0 para eliminado
};

// Estructura para la agenda
struct Agenda {
    struct Event events[MAX_EVENTS];
    int count;
};

// Prototipos de funciones
int isValidDate(int year, int month, int day);
int isEventUpcoming(struct Event event);
void sortEventsByProximity(struct Agenda *agenda, int *indices, int *count);
int addEvent(struct Agenda *agenda, const char *title, const char *description, int year, int month, int day, int hour, int minute);
int deleteEvent(struct Agenda *agenda, int index);
int editEvent(struct Agenda *agenda, int index, const char *description, int year, int month, int day, int hour, int minute);
void printMessage(int msgIndex, ...); // Ajuste para argumentos variables
void initAgenda(struct Agenda *agenda); // Declaración

#endif
#include <stdio.h>
#include <gtk/gtk.h>
#include "agenda.h"
#include "gui.h"

int main(int argc, char *argv[]) {
    struct Agenda agenda;
    initAgenda(&agenda); // Llama a la función definida en agenda.c
    initGUI(argc, argv, &agenda);
    return 0;
}
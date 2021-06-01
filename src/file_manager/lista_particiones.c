#include <stdlib.h>
#include <stdio.h>
#include "lista_particiones.h"

Particion *process_init(int id, long int id_abs, int cantidad_de_bloques)
{
    Particion *particion = calloc(1, sizeof(Particion));
    particion->id = id;
    particion->id_abs = id_abs;
    particion->cantida_de_bloques = cantidad_de_bloques;
    particion->next = NULL;

    return particion;
}

/** Inicializa una lista vacía */
Lista *lista_init()
{
    Lista *list = calloc(1, sizeof(Lista));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void sortedInsert(Lista *list, Particion *new_particion)
{
    Particion *current;
    /* Special case for the head end */
    if (list->head == NULL)
    {
        list->head = new_particion;
        list->tail = new_particion;
    }
    else if (list->head->id_abs >= new_particion->id_abs)
    {
        new_particion->next = list->head;
        list->head = new_particion;
    }
    else if (list->tail->id_abs <= new_particion->id_abs)
    {
        list->tail->next = new_particion;
        list->tail = new_particion;
    }
    else
    {
        /* Locate the node before the
        point of insertion */
        current = list->head;
        while (current->next->id_abs < new_particion->id_abs)
        {
            current = current->next;
        }
        new_particion->next = current->next;
        current->next = new_particion;
    }
}

void lista_destroy(Lista *list)
{
    // Liberamos los nodos
    if (list->head)
    {
        Particion *curr = list->head->next;
        Particion *prev = list->head;

        while (curr)
        {
            free(prev);
            prev = curr;
            curr = curr->next;
        }
        free(prev);
    }
    // Liberamos la lista en si
    free(list);
}
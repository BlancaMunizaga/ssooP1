#pragma once

struct particion_node;
/* struct particion*/
typedef struct particion_node Particion;
struct particion_node
{
    int id;
    int id_abs;
    int cantida_de_bloques;
    Particion *next;
};

struct lista;
// Definimos un alias para el struct lista
typedef struct lista Lista;
// Definimos el struct lista
struct lista
{
    /** El primer nodo de la lista */
    Particion *head;
    Particion *tail;
};

Particion *process_init(int id, int id_abs, int cantidad_de_bloques);
/** Inicializa una lista vacía */
Lista *lista_init();
void sortedInsert(Lista *list, Particion *new_particion);
void lista_destroy(Lista *list);
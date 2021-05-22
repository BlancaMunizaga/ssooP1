#pragma once

// Declaramos el struct para el process
struct entrada_mbt;
/* struct process*/
typedef struct entrada_mbt entradaMBT;
struct entrada_mbt
{
    /* si el primer bite es > 127 la partición es valida  1 es valido 0 es invalido*/
    int validez;
    int id_particion;
    int identificador_absoluto;
    // Es el primer bloque de la particion, que es el bloque directorio
    int cantidad_bloques;
};
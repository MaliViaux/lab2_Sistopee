#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h> 
#include "estructura.h"

// CABECERAS DE FUCNIONES

// Entradas: char
// Salida: int
// Descripcion: define si una cadena es regular o no, 1 o 0
int esCadenaRegular(char *cadena);

// Entradas: void
// Salida: lista
// Descripcion: crea una lista enlazada
Lista *crearLista();


// Entradas: char
// Salida: Nodo
// Descripcion: crea un Nodo con contenido char
Nodo *crearNodo(char val);


// Entradas: Lista
// Salida: Nodo
// Descripcion: retorna el ultimo nodo de una lista enlazada
Nodo *ultimo(Lista *L);


// Entradas: Lista X char
// Salida: void
// Descripcion: inserta nodo con contenido char dado al final de una lista enlazada
void insertarFin(Lista *L, char val);


// Entradas: Lista
// Salida: void
// Descripcion: elimina el primer nodo de una lista enlazada
void eliminarIni(Lista *L);


// Entradas: lista
// Salida: void
// Descripcion: recorre y muestra por consola una lista enlazada
// void recorrerLista(Lista *L);

void recorrerLista(Lista *L, FILE *fp, int option);

// Entrada: lista
// Salidas: int o una funcion
// Descripcion: casos posibles en el estado 2
int estado2(Lista *L);


// Entrada: lista
// Salidas: int o una funcion
// Descripcion: casos posibles en el estado 1
int estado1(Lista *L);


// Entrada: lista
// Salidas: int o una funcion
// Descripcion: casos posibles en el estado 3
int estado3(Lista *L);


// Entrada: lista
// Salidas: int o una funcion
// Descripcion: inicia las recursiones, caso base de lista vacia retorna true, caso contrario entra al estado 1
int esRegular(Lista *L);
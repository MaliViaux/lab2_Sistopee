#include <stdio.h>
#include <stdlib.h>

//TDA LISTA ENLAZADA
typedef struct nodo{
	char dato;
	struct nodo *next;
}Nodo;

typedef struct lista{
	Nodo *head;
	int n;
}Lista;
#include "fworker.h"

// Entradas: void
// Salida: lista
// Descripcion: crea una lista enlazada
Lista *crearLista(){
    Lista *L;
    L = (Lista*)malloc(sizeof(Lista));
    L -> head = NULL;
    L -> n = 0;
    return L;
}

// Entradas: cadena
// Salida: 0,1 int
// Descripcion: Verifica si es una cadena
int esCadenaRegular(char *cadena){
    Lista *L = crearLista();
    for (int i = 0; i < 60; i++) {
        insertarFin(L, (char) cadena[i]);
    }
    return esRegular(L);
}

// Entradas: char
// Salida: Nodo
// Descripcion: crea un Nodo con contenido char
Nodo *crearNodo(char val){
	Nodo *nodN;
	nodN = (Nodo*)malloc(sizeof(Nodo));
	nodN -> dato = val;
	nodN -> next = NULL;
	return nodN;
}

// Entradas: Lista
// Salida: Nodo
// Descripcion: retorna el ultimo nodo de una lista enlazada
Nodo *ultimo(Lista *L){
	Nodo *N;
    N = L -> head;
    while (N -> next != NULL ){
        N = N -> next;
    }
    return N;
}

// Entradas: Lista X char
// Salida: void
// Descripcion: inserta nodo con contenido char dado al final de una lista enlazada
void insertarFin(Lista *L, char val){
	Nodo *N;
	N = crearNodo(val);
	if (L -> head == NULL){
		L -> head = N;
	}
	else{
        Nodo *aux;
        aux = ultimo(L);
        aux -> next = N;
	}
	L -> n += 1;
}

// Entradas: Lista
// Salida: void
// Descripcion: elimina el primer nodo de una lista enlazada
void eliminarIni(Lista *L){
    if (L -> head != NULL){
        Nodo *aux;
        aux = L ->head;
        L->head = L->head->next;
        aux->next = NULL;
        free(aux);
        L->n -= 1;
    }
}

// Entradas: lista
// Salida: void
// Descripcion: recorre y muestra por consola una lista enlazada
// void recorrerLista(Lista *L){
//     Nodo *aux;
//     aux = L -> head;
//     while (aux != NULL){
//         printf("%c", aux -> dato);
//         aux = aux -> next;
//     }
// }

void recorrerLista(Lista *L, FILE *fp, int option){
    Nodo *aux;
    aux = L -> head;
    while (aux != NULL){
        
        if (option == 1){
            printf("%c", aux -> dato);
        }
        
        fprintf(fp, "%c", aux -> dato);
      
        aux = aux -> next;
    }
}

// ----------FUNCIONES RECURSIVAS PARA RECONOCER LA EXPRESION REGULAR----------
// Entrada: lista
// Salidas: int o una funcion
// Descripcion: inicia las recursiones, caso base de lista vacia retorna true, caso contrario entra al estado 1
int esRegular(Lista *L){
    // caso base, si la lista es vacia retornar 1 (true)
    if (L->head == NULL){
        return 1;
    }
    // caso contrario entrar al estado 1.
    else{
        return estado1(L); // llamada recursiva
    }
}

// Entrada: lista
// Salidas: int o una funcion
// Descripcion: casos posibles en el estado 1
int estado1(Lista *L){
    // se llegó al final de la cadena y no se encuentra la expresion regular, retornar un false, se resuelven las recursiones
    if (L->head == NULL){
        return 0;
    }
    // si la letra actual es una G avanzar al estado 2
    if (L->head->dato == 'G'){
        eliminarIni(L); // se elimina el caracter examinado
        return estado2(L); // llamada recursiva
    }
    // si la letra actual es una A,C,T permanecer en el estado 2.
    if (L->head->dato == 'A' || L->head->dato == 'C' || L->head->dato == 'T'){
        eliminarIni(L); // se elimina el caracter examinado
        return estado1(L); // llamada recursiva
    }
    return 0;
}

// Entrada: lista
// Salidas: int o una funcion
// Descripcion: casos posibles en el estado 2
int estado2(Lista *L){
    // se llegó al final de la cadena y no se encuentra la expresion regular, retornar un false, se resuelven las recursiones
    if (L->head == NULL){
        return 0;
    }
    // si la letra actual es una T avanzar al estado 3.
    if (L->head->dato == 'T'){
        eliminarIni(L); // se elimina el caracter examinado
        return estado3(L); // llamada recursiva
    }
    // si la letra actual es una G volver al estado 2 (ya cumplio el estado 1)
    if (L->head->dato == 'G'){
        eliminarIni(L); // se elimina el caracter examinado
        return estado2(L); // llamada recursiva
    }
    // si la letra actual es una A,C volver al estado 1.
    if (L->head->dato == 'A' || L->head->dato == 'C'){
        eliminarIni(L); // se elimina el caracter examinado
        return estado1(L); // llamada recursiva
    }
    return 0;
}

// Entrada: lista
// Salidas: int o una funcion
// Descripcion: casos posibles en el estado 3
int estado3(Lista *L){
    // se llegó al final de la cadena y no se encuentra la expresion regular, retornar un false, se resuelven las recursiones
    if (L->head == NULL){
        return 0;
    }
    // si la letra actual es una T permanecer en el estado 3.
    if (L->head->dato == 'T'){
        eliminarIni(L); // se elimina el caracter examinado
        return estado3(L); // llamada recursiva
    }
    // si la letra actual es una C se encuentra la expresion, se retorna un true y se resuelven las recursiones
    if (L->head->dato == 'C'){
        eliminarIni(L);
        return 1;
    }
    // si la letra actual es una G volver al estado 2 (ya cumplio el estado 1)
    if (L->head->dato == 'G'){
        eliminarIni(L); // se elimina el caracter examinado
        return estado2(L); // llamada recursiva
    }
    // si la letra actual es una A volver al estado 1.
    if (L->head->dato == 'A'){
        eliminarIni(L); // se elimina el caracter examinado
        return estado1(L); // llamada recursiva
    }
    return 0;
}

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "estructura.h"
#include <sys/wait.h>
int main(int argc, char *argv[])
{
	// Semilla
	srand(time(NULL));
	// variables para los parametros de entrada
	char nombre_archivo_salida[100];
	strcpy(nombre_archivo_salida, argv[2]);
	char nombre_archivo_entrada[100];
	strcpy(nombre_archivo_entrada, argv[1]);
	char procesoPorPantalla[100];
	strcpy(procesoPorPantalla, argv[5]);
	int cantidadWorkers = atoi(argv[4]);
    //int cantidadChunks = atoi(argv[3]);

	// pid de los procesos hijos;
	int workers_pid[cantidadWorkers];

	// pipes•••••••••••
	int fd_padre_hijo[cantidadWorkers][2]; // padre escritura 1
	int fd_hijo_padre[cantidadWorkers][2]; // padre lectura 0

	// cantidad de pipes por proceso que es igual al numero de workers
	for (int cont = 0; cont < cantidadWorkers; cont++)
	{
		pipe(fd_padre_hijo[cont]);
		pipe(fd_hijo_padre[cont]);
	}
	
	// creacion de los procesos hijos
	for (int cont = 0; cont < cantidadWorkers; cont++)
	{
		workers_pid[cont] = fork();
		// condición para cuando existe un error el proceso hijo.
		if (workers_pid[cont] == -1)
		{
			printf("Error al crear el proceso hijo");
			exit(1);
		}
		else if (workers_pid[cont] == 0)
		{
			// proceso hijo
			// se cierran las entradas a los pipes inecesarias
			close(fd_padre_hijo[cont][1]);
			close(fd_hijo_padre[cont][0]);
			// dup para que el padre le escriba al hijo
			if (dup2(fd_padre_hijo[cont][0], STDIN_FILENO) == -1)
			{
				printf("Error al duplicar el descriptor de archivo");
				exit(1);
			}
			// dup para que el hijo le escriba al padre
			if (dup2(fd_hijo_padre[cont][1], STDOUT_FILENO) == -1)
			{
				printf("Error al duplicar el descriptor de archivo");
				exit(1);
			}
			// cierre de pipes dado que se realizo el dup
			close(fd_padre_hijo[cont][0]);
			close(fd_hijo_padre[cont][1]);
			// ejecucion de los procesos worker
			execlp("./worker", "worker", NULL);
		}
	}
	// cierre de entradas del pipe para el padre inecesarias
	for (int i = 0; i < cantidadWorkers; i++)
	{
		close(fd_padre_hijo[i][0]);
		close(fd_hijo_padre[i][1]);
	}

	for(int i = 0; i < cantidadWorkers; i++)
	{
		int status;
		waitpid(workers_pid[i], &status, 0);
	}

	// Se abre el archivo.
	FILE *archivo = fopen(nombre_archivo_entrada, "r");
	char buffer[60];
	// lectura del archivo
	while (fgets(buffer, 60, archivo) != NULL)
	{
		// se escoge un hijo de forma aleatoria
		int hijo = rand() % cantidadWorkers;
		printf("%s\n",buffer);
		// Se escribe la linea al hijo
		if ((write(fd_padre_hijo[hijo][1], buffer, 60)) == -1)
		{
			printf("Error al escribir en el pipe");
			exit(1);
		}
	}

	// Se escribe el FIN para que dejen de leer los workers
	for (int i = 0; i < cantidadWorkers; i++)
	{
		printf("FIN");
		if ((write(fd_padre_hijo[i][1], "FIN", sizeof("FIN")) == -1))
		{
			printf("Error al escribir en el pipe");
			exit(1);
		}
	}

	// LECTURA DE LAS CADENAS LEIDAS POR CADA HIJO
    int cantidadLineas = 0;
	int SI = 0;
	int NO = 0;
	int temp;
	int temp1;
	for (int i = 0; i < cantidadWorkers; i++){
		read(fd_hijo_padre[i][0], &temp, sizeof(temp));
		cantidadLineas += temp;
		printf("cant lineas %d\n",cantidadLineas);
        read(fd_hijo_padre[i][0], &temp1, sizeof(temp));
		NO += temp1;
		printf("cant no %d\n",NO);
        read(fd_hijo_padre[i][0], &temp1, sizeof(temp));
		SI += temp1;
		printf("cant si %d\n",SI);
	}

	// Arreglo que contendra todos las cadenas y si es regular o no por worker
    char **lineas = (char **)malloc(sizeof(char *) * (cantidadLineas));
	for (int i = 0; i < cantidadWorkers; i++)
	{
		lineas[i] = (char *)malloc(sizeof(char) * 70);
	}

	// LECTURA DE CADENAS Y SI ES REGULAR O NO
	char *temp2;
	for (int i = 0; i < cantidadWorkers; i++)
	{
		read(fd_hijo_padre[i][0], &temp2, sizeof(lineas));
		lineas[i] = temp2;
		printf(" %ds\n",lineas[i]);
	}

	// ESCRITURA DEL ARCHIVO FINAL
	FILE *archivoSalida = fopen(nombre_archivo_salida, "w");
	for (int i = 0; i < cantidadLineas; i++)
	{
		fprintf(archivoSalida,"%s\n",lineas[i]);
	}
    fprintf(archivoSalida,"Total de expresiones que Si son regulares: %d\n",SI);
    fprintf(archivoSalida,"Total de expresiones que No son regulares: %d\n",NO);
    fprintf(archivoSalida,"Total de lineas leidas: %d\n",cantidadLineas);
	fclose(archivoSalida);
	
	// IMPRESION POR PANTALLA EN CASO DE QUE EXISTA LA FLAG
	if (atoi(procesoPorPantalla) == 1)
	{
		for (int i = 0; i < cantidadLineas; i++)
		{
			printf("%s\n",lineas[i]);
			
		}
		printf("Total de expresiones que Si son regulares: %d\n",SI);
    	printf("Total de expresiones que No son regulares: %d\n",NO);
    	printf("Total de lineas leidas: %d\n",cantidadLineas);
	}

	for (int i = 0; i < cantidadLineas; i++)
	{
		free(lineas[i]);
	}
	free(lineas);
	return 0;
}
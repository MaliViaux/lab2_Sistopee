#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
//#include <sys/wait.h> DESCOMENTAR ESTO

int main(int argc, char* argv[]){
	extern char *optarg; //Almacena los argumentos introducidos después de haber indicado la variable.
    extern int optind; // Almacena el número de argumentos introducidos.
    int option;
    int iflag = 0;
    int oflag = 0;
    int error = 0;
    // Variables de entrada
    char nombre_archivo_salida[100];
    char nombre_archivo_entrada[100];
    char cantidadWorkers[100] = "1";
    char cantidadChunks[100] = "1";
    char porConsola[100] = "0";
    while((option = getopt(argc, argv, "i:o:c:n:b")) != -1){
        switch(option){
            case 'i':
                iflag = 1;
                strcpy(nombre_archivo_entrada,optarg);
                break;
            case 'o':
                oflag = 1;
                strcpy(nombre_archivo_salida,optarg);
                break;
            case 'c':
                strcpy(cantidadChunks,optarg);
                break;
            case 'n':
                strcpy(cantidadWorkers,optarg);
                break;
            case 'b':
                strcpy(porConsola,"1");
                break;
            case '?':
                error = 1;
                break;
            exit(0);
        }
    }
    // Verificaciones de archivo de entradas
    if(iflag!=1){
        printf("Por favor ingresar nombre del archivo de entrada\n");
        return 0;
    }
    // Verificacion de archivo de salida
    if(oflag!=1){
        printf("Por favor ingresar nombre del archivo de salida\n");
        return 0;
    }
    // Verificacion de error
    if(error==1){
        printf("Por favor ingrese las variables de entrada\n");
        return 0;
    }
    // verificacion cantidad de workers
    if(atoi(cantidadWorkers)<1){
        printf("Por favor ingresar un numero de workers mayor a 0\n");
        return 0;
    }
    // verificacion cantidad de chunks
    if(atoi(cantidadChunks)<1){
        printf("Por favor ingresar un numero de chunks mayor a 0\n");
        return 0;
    }
    // Se ejecuta el broker
    int pid= fork();
    if(pid==0){
        execlp("./broker", "broker", nombre_archivo_entrada,nombre_archivo_salida, cantidadWorkers,cantidadChunks,NULL);
    }
        
    return 0;
}
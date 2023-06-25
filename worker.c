#include "fworker.h"

int main(int argc, char *argv[])
{
   // LECTURA WORKERS
    char buff[2000];
    int contador = 0;
    char lectura[300000] = "";
    char *puntero_string;
    // Lectura de lineas enviadas por el broker
    while (1)
    {
        read(STDIN_FILENO, buff, 2000);
        if (strcmp(buff, "FIN") == 0)
        {
            break;
        }
        strcat(lectura, buff);
        strcat(lectura, "/");
        contador++;
    }

    // declaracion arreglo de lineas
    char **lineas = (char **)malloc(sizeof(char *) * (contador));
    // asignando memoria a el arreglo de lineas
    for (int i = 0; i < contador; i++)
    {
        lineas[i] = (char *)malloc(sizeof(char) * 500);
    }
    puntero_string = strtok(lectura, "/");
    // Se agregan las lineas a un arreglo de lineas
    for (int i = 0; i < contador; i++)
    {
        strcpy(lineas[i], puntero_string);
        puntero_string = strtok(NULL, "/");
    }

    char *respuestas[contador];
    int SI = 0;
    int NO = 0;
    for (int i = 0; i < contador; i++)
    {   
        if (esCadenaRegular(lineas[i]) == 1)
        {
            respuestas[i] = "si";
            SI++;
        }
        else
        {
            respuestas[i] = "no";
            NO++;
        }
    }

    // ENVIO DE CANTIDAD DE LINEAS LEIDAS POR EL WORKER
    if (write(STDOUT_FILENO, &contador, sizeof(contador)) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
    // ENVIO DE CANTIDAD DE EXPRESIONES NO REGULARES
    if (write(STDOUT_FILENO, &NO, sizeof(NO)) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
    // ENVIO DE CANTIDAD DE EXPRESIONES REGULARES
    if (write(STDOUT_FILENO, &SI, sizeof(SI)) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
    // Envio de la cadena leida y la respuesta (si/no) por el worker
    for(int i=0; i<contador;i++){
        if(write(STDOUT_FILENO,lineas[i],strlen(lineas[i])+1) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
        char space = ' ';
        if(write(STDOUT_FILENO, &space, 1) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
        if(write(STDOUT_FILENO,respuestas[i],strlen(respuestas[i])+1) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
        char newline = '\n';
        if(write(STDOUT_FILENO, &newline, 1) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
    }
    for (int i = 0; i < contador; i++)
    {
        free(lineas[i]);
    }
    free(lineas);
    return 0;
}
/** Autor: @DarioNorrigan **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "partidas.h"

#define TAM 4    // El valor de TAM-1 define el tamaño del tablero, puedes cambiarlo a 4<=TAM<=7
#define TIEMPO 3 // Puede cambiar este valor para modificar el intervalo entre jugadas al reproducir una partida 1=1segundo

int tam = TAM;
int tiempo = TIEMPO;

int menu()
{
    limpiarPantalla();
    char opcion[4];
    int reto=1;

    printf("\t\tBuscaminas de 2 jugadores\n\n");
    printf("1. Nueva partida\n");
    printf("2. Mejores puntajes\n");
    printf("3. Reproducir partida\n");
    printf("4. Información acerca del juego\n");
    printf("5. Salir");
    printf("\n\tOpcion: ");
    fgets(opcion, 4, stdin);    opcion[strlen(opcion)-1] = '\0';
    fflush(stdin);
    if(!strcmp(opcion, "1"))
    {
        Jugador jug1, jug2;
        Tablero nuevo[TAM][TAM];
        int i=1;

        crearTablero(nuevo);
        regisJugadores(&jug1, &jug2);

        while(1)
        {
            if(reto==0)
                break;
            imprimir(tam, nuevo);

            reto = jugada(i, &jug1, &jug2, nuevo);
            i++;
            if(i>2)
                i=1;
        }
    }
    else if(!strcmp(opcion, "2"))
    {
        mejores();
    }
    else if(!strcmp(opcion, "3"))
    {
        partidasJugadas();
    }
    else if(!strcmp(opcion, "4"))
    {
        asistencia();
    }
    else if(!strcmp(opcion, "5"))
        return 0;
    return 1;
}

int main()
{
    setlocale(LC_ALL, "spanish");
    int es;

    do{
        es=menu();
    }while(es);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <string.h>
#include "arboles.h"

int menu();

int main()
{
    setlocale(LC_ALL, "spanish");
    system("color 0F");
    int es;

    do{
        es = menu();
    }while(es);

    return 0;
}

int menu()
{
    char opc;
    static int ini=1; // 1= Diccionario no inicializado

    limpiarPant();
    printf("\t\tDiccionario en lenguaje C\n\n");
    printf("a. Inicializar diccionario.\n");
    printf("b. Agregar una palabra en el diccionario.\n");
    printf("c. Eliminar una palabra contenida en el diccionario.\n");
    printf("d. Mostrar toda la información del diccionario.\n");
    printf("e. Buscar la información de una palabra.\n");
    printf("f. Mostrar todas las palabras repetidas.\n");
    printf("g. Salir (libera la memoria)\n");
    printf("\n\tSelecciona una opción: ");
    scanf("%c", &opc);

    switch(opc)
    {
    case 'a':   inicializar(ini); ini=0;
        break;
    case 'b':
        if(ini) goto error;
        agregar_eliminar(1);
        break;
    case 'c':
        if(ini) goto error;
        agregar_eliminar(0);
        break;
    case 'd':
        if(ini) goto error;
        mostrarPalabras();
        break;
    case 'e':
        infoPalabra(1);
        if(ini) goto error;
        break;
    case 'f':
        infoPalabra(0);
        if(ini) goto error;
        break;
    case 'g':   fin();  return 0;
        break;
    }
    return 1;

    error:
        printf("\nEl diccionario aún no ha sido inicializado.\n\n");
        system("pause");
    return 1;
}

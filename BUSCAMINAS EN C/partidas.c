#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "partidas.h"

static Nodo *raiz = NULL, *cola = NULL; // Declaración de la raiz y cola de la lista

extern int tam;

void limpiarPantalla()
{
    system("cls");
}

/*
    Se almacenan los nombres de los dos jugadores y se les asigna una cantidad de vidas
    que por default son 2.
*/
void regisJugadores(Jugador *j1, Jugador *j2)
{
    limpiarPantalla();
    setlocale(LC_ALL, "spanish");
    printf("¿Cuál es el nombre del jugador 1? ");
    fgets(j1->nombre, 20, stdin);   j1->nombre[strlen(j1->nombre)-1] = '\0';

    printf("¿Cuál es el nombre del jugador 2? ");
    fgets(j2->nombre, 20, stdin);   j2->nombre[strlen(j2->nombre)-1] = '\0';

    j1->vidas = j2->vidas = 2; // Puedes cambiar este valor tambien
}

void crearTablero(Tablero tab[tam][tam])
{
    if(tam < 4 || tam > 7)
    {
        printf("\nEl tamaño del tablero es inválido, puede cambiarlo desde el código fuente en main.c\n");
        exit(1);
    }

    int i, j, x, minas = 2*tam-4, auxNums = 48, auxLetras = 97;
    srand(time(NULL));

    for(i=0; i<tam; ++i)
    {
        for(j=0; j<tam; ++j)
        {
            if(i==0)
            {
                tab[i][j].item = (char)auxNums;
                auxNums++;
            }
            else if(j==0)
            {
                tab[i][j].item = (char)auxLetras;
                auxLetras++;
            }
            else
                tab[i][j].item = '*';

            tab[i][j].mina = 0;
        }
    }
    tab[0][0].item = ' ';

    faltan:
        for(i=1; i<tam; ++i)
        {
            for(j=1; j<tam; ++j)
            {
                if(minas == 0)
                    goto yaa;
                if(rand()%2==1)
                {
                    x=-1;
                    minas--;
                }
                else
                    x=0;
                if(tab[i][j].mina==-1) // -1 es una mina
                    minas++;

                tab[i][j].mina= x;
            }
        }

    if(minas>0)
        goto faltan;
    yaa:

    for(i=1; i<tam; ++i)
        for(j=1; j<tam; ++j)
            if(tab[i][j].mina == 0)
            {
                for(int k=i-1; k<i+2; ++k)
                    for(int l=j-1; l<j+2; ++l)
                        if(tab[k][l].mina== -1)
                            tab[i][j].mina++;   // Suma los contadores la cantidad de minas que
                                                // hay al lado de cada uno
            }
}

/*
    Recibe un tablero, y el tamaño del mismo 't'
    Esto porque al reproducir una partida, necesita el tamaño de ese anterior tablero.
*/
void imprimir(int t, Tablero tab[t][t])
{
    int i, j;
    limpiarPantalla();
    for(i=0; i<t; ++i)
    {
        printf("\t\t");
        for(j=0; j<t; ++j)
        {
            printf(DEFAULT_COLOR);
            if(tab[i][j].item == 'x')
                printf(RED);
            else if(i==0 || j==0)
                printf(S_YELLOW);
            printf("%c  ", tab[i][j].item);
        }
        printf("\n");
    }
}

/* Funcion para insertar las jugadas en la cola */
static void insCola(char jugada[4])
{
    Nodo *nuevo = malloc(sizeof(Nodo));
    strcpy(nuevo->jugada, jugada);
    nuevo->sig=NULL;

    if(!raiz)
    {
        raiz=nuevo;
        cola=nuevo;
    }
    else
    {
        cola->sig=nuevo;
        cola=nuevo;
    }
}

/*
    Si la jugada ya había sido digitada retorna 0 sino:
    -1 significa que había una mina
    1 significa que no había mina
    El úlitmo parametro solo puede tomar dos valores:
    1 = nueva partida- por lo que inserta las jugadas en la lista tipo cola
    0 = reproducir partida- por lo que no inserta las jugadas en la cola
*/
static int descubrir(int x, int y, int t, Tablero tab[t][t], char jug[4], int func)
{
    if(tab[x][y].item == '*')
    {
        if(func)
            insCola(jug);
        if(tab[x][y].mina == -1)
        {
            tab[x][y].item = 'x';
            return -1;
        }
        else
            tab[x][y].item = (char)(tab[x][y].mina + 48);
        return 1;
    }
    else
        return 0;
}

/*
    Contar la cantidad de nodos (jugadas) es importante para saber cuántos guardaremos en el
    archivo binario.
*/
static int cantNodos()
{
    Nodo *reco=raiz;
    int cont=0;

    while(reco!=NULL)
    {
        cont++;
        reco=reco->sig;
    }
    return cont;
}

/*
    Esta funcion tiene la finalidad de:
    1. Calcular y mostrar puntajes al final de una partida
    2. SOLO SI el archivo existe previamente, busca coincidencias en los nombres ya existentes
       en el archivo y suma su puntaje actual con los anteriores obtenidos
    3. Si no encuentra los nombres en el archivo, da por hecho que no existen aún jugadores
       registrados con ese nombre y simplemente los añade al archivo.
*/
static void finalPartida(Jugador *j1, Jugador *j2, int jugadas, Tablero tab[tam][tam])
{
    limpiarPantalla();
    imprimir(tam, tab);
    j1->puntaje = j1->vidas*jugadas;    // Primero calculamos el puntaje de ambos
    j2->puntaje = j2->vidas*jugadas;

    printf("Puntaje de %s: %d\n", j1->nombre, j1->puntaje);
    printf("Puntaje de %s: %d\n", j2->nombre, j2->puntaje); // Y lo mostramos en pantalla

    FILE *fp = fopen(PUNTAJES, "r+b");
    if(fp==NULL)
        goto sig; // Verifica si existe el archivo para saber si va a modificar los datos de un jugador

    Jugador aux;

    fread(&aux, sizeof(Jugador), 1, fp);
    while(!feof(fp))
    {
        if(strcmp(j1->nombre, aux.nombre)==0)
        {
            j1->puntaje+=aux.puntaje;

            fseek(fp, -28, SEEK_CUR);

            fwrite(j1, sizeof(Jugador), 1, fp);
            fflush(fp);
            j1->puntaje=-1; // Esto es un marcador para saber si ya fue insertado su puntaje.
        }
        else if(strcmp(j2->nombre, aux.nombre)==0)
        {
            j2->puntaje+=aux.puntaje;

            fseek(fp, -28, SEEK_CUR);

            fwrite(j2, sizeof(Jugador), 1, fp);
            fflush(fp);
            j2->puntaje=-1; // Esto es un marcador para saber si ya fue insertado su puntaje.
        }
        fread(&aux, sizeof(Jugador), 1, fp);
    }
    fclose(fp);
    sig:                                                /**SIG**/

    // A partir de aquí surge el error.
    fp = fopen(PUNTAJES, "ab");
    if(fp==NULL)
        exit(3);
    if(j1->puntaje != -1)
        fwrite(j1, sizeof(Jugador), 1, fp);
    if(j2->puntaje != -1)
        fwrite(j2, sizeof(Jugador), 1, fp);

    fclose(fp);

    fp = fopen(PARTIDAS, "ab");
    if(fp==NULL)
        exit(3);
    int n = cantNodos(), tamTablero = tam*tam;

    // Para cada partida guarda en este orden
    fwrite(&n, sizeof(int), 1, fp);               // Cantidad de jugadas totales de la partida
    fflush(fp);
    fwrite(&tamTablero, sizeof(int), 1, fp);      // El tamaño de la matriz del tablero
    fwrite(j1->nombre, sizeof(char), 20, fp);     // Nombre del jugador 1
    fflush(fp);
    fwrite(j2->nombre, sizeof(char), 20, fp);     // Nombre del jugador 2
    fflush(fp);

    Nodo *reco=raiz, *del;
    while(reco != NULL)
    {
        fwrite(reco->jugada, sizeof(char), 4, fp); // La información de todos los nodos de la partida
        fflush(fp);
        del=reco;
        reco=reco->sig;
        free(del);
    }
    fwrite(tab, sizeof(Tablero), tamTablero, fp);// Por último la matriz numérica del tablero
    fflush(fp);

    fclose(fp);
    raiz= cola =NULL;
}

/*
    Si ya solo quedan minas retorna 0
    de lo contrario retorna 1
*/
static int hayMinas(Tablero tab[tam][tam])
{
    int i, j, contMinas=0, contCasillasRestantes=0;

    for(i=1; i<tam; ++i)
    {
        for(j=1; j<tam; ++j)
        {
            if(tab[i][j].item == '*')
            {
                if(tab[i][j].mina == -1)
                    contMinas++;
                contCasillasRestantes++;
            }
        }
    }
    if(contMinas == contCasillasRestantes) return 0;
    else return 1;
}

int jugada(int turno, Jugador *j1, Jugador *j2, Tablero tab[tam][tam])
{
    static int numJugada=1;
    int reto;
    char opc[4], *jugadorActual;

    if(turno == 1)
        jugadorActual = j1->nombre;
    else
        jugadorActual = j2->nombre;

    printf("\t\tVidas %s: %d   Vidas %s: %d\n", j1->nombre, j1->vidas, j2->nombre, j2->vidas);
    no:
    printf("Jugador %d, %s, turno: %d, digita la coordenada que seleccionaste: ", turno, jugadorActual, numJugada);
    fgets(opc, 4, stdin);   opc[strlen(opc)-1] = '\0';
    fflush(stdin);

    for(int i=1; i<tam; ++i)
        if(tab[i][0].item == opc[0])     // Si la primer coordenada se encuentra
            for(int j=1; j<tam; ++j)
                if(tab[0][j].item == opc[1])     // Si la segunda se encuentra
                {
                    reto = descubrir(i, j, tam, tab, opc, 1);
                    if(reto == 0)
                        goto no;    // Si ya se digitó antes, vuelve a la linea...
                    else
                    {
                        if(reto==-1) (turno==1) ? (j1->vidas--) : (j2->vidas--);
                        goto si;
                    }
                }
    si:
    if(j1->vidas == 0 || j2->vidas == 0 || !hayMinas(tab))
    {
        finalPartida(j1, j2, numJugada, tab);
        if(!hayMinas(tab))
            printf("¡Solo quedan minas!\n");
        else
            printf("¡%s se quedó sin vidas!\n", !j1->vidas ? j1->nombre : j2->nombre);

        printf("\nFin del juego\n");
        system("pause");
        limpiarPantalla();
        return 0;
    }
    if(turno==2) numJugada++;

    return 1;
}


void mejores()
{
    limpiarPantalla();
    FILE *fp = fopen(PUNTAJES, "rb");
    if(fp==NULL)
        printf("Nada por aquí aún...\n");
    else
    {
        Jugador aux;

        fread(&aux, sizeof(Jugador), 1, fp);

        while(!feof(fp))
        {
            printf("%s, %d\n", aux.nombre, aux.puntaje);
            fread(&aux, sizeof(Jugador), 1, fp);
        }
        fclose(fp);
    }

    system("pause");
    limpiarPantalla();
}
/*
    Reproduce la partida y libera la memoria solicitada por la función "partidasJugadas()"
*/
static void reproducir(char *nm1, char *nm2, int t, Tablero tab[t][t])
{
    limpiarPantalla();
    Nodo *reco=raiz, *del;
    int i, j, turno=0;
    extern int tiempo;

    for(i=1; i<t; ++i)
        for(j=1; j<t; ++j)
            tab[i][j].item = '*';

    imprimir(t, tab);

    while(reco!=NULL)
    {
        printf("%s - %s", reco->jugada, turno? nm2 : nm1);
        turno++;
        if(turno>1)
            turno=0;
        sleep(tiempo);
        for(int x=1; x<t; ++x)
            if((char)x+96 == reco->jugada[0])
                i = x;
        j = reco->jugada[1]-48;

        descubrir(i, j, t, tab, reco->jugada, 0);
        del=reco;
        reco=reco->sig;
        free(del);
        imprimir(t, tab);
    }
    raiz= cola =NULL;
}

/*
    Primero muestra las partidas jugadas, y luego te deja seleccionar una opción y
    si está disponible, la reproduce.
*/
void partidasJugadas()
{
    limpiarPantalla();
    FILE *fp = fopen(PARTIDAS, "rb");
    if(!fp)
        printf("Nada por aquí aún...\n");

    else
    {
        char n1[20], n2[20], jugada[4];
        int nodos, tamTab, sqrtTamTab, num=1, opc;

        fread(&nodos, sizeof(int), 1, fp);
        while(!feof(fp))    // Imprime primero las partidas disponibles
        {
            fread(&tamTab, sizeof(int), 1, fp);
            fread(n1, sizeof(char), 20, fp);
            fread(n2, sizeof(char), 20, fp);
            fseek(fp, nodos*4, SEEK_CUR);       // Hacemos un brinco de los nodos
            fseek(fp, tamTab*sizeof(Tablero), SEEK_CUR); // Hacemos otro brinco, pero ahora del tablero
            printf("%d. %s vs %s\n",num, n1, n2);
            fread(&nodos, sizeof(int), 1, fp);
            num++;
        }
        fclose(fp);
        printf("Digite el número de la partida:\n> ");
        scanf("%d", &opc);

        if(opc >= num)  // Digitó una opción inválida
            goto next;

        fclose(fp);
        fp = fopen(PARTIDAS, "rb");
        if(!fp)
            exit(5);

        fread(&nodos, sizeof(int), 1, fp);

        while(opc != 1)
        {
            fread(&tamTab, sizeof(int), 1, fp);
            fseek(fp, 40, SEEK_CUR);
            fseek(fp, nodos*4, SEEK_CUR);    // Luego la busca de nuevo para reproducirla
            fseek(fp, tamTab*sizeof(Tablero), SEEK_CUR);
            fread(&nodos, sizeof(int), 1, fp);
            opc--;
        }
        fread(&tamTab, sizeof(int), 1, fp);
        fread(n1, sizeof(char), 20, fp);    // Guardamos los nombres de los dos jugadores
        fread(n2, sizeof(char), 20, fp);

        while(nodos != 0)
        {
            fread(jugada, sizeof(char), 4, fp);
            insCola(jugada);
            nodos--;
        }
        sqrtTamTab = sqrt(tamTab);
        Tablero tab[sqrtTamTab][sqrtTamTab];

        fread(tab, tamTab*sizeof(Tablero), 1, fp);
        fclose(fp);

        reproducir(n1, n2, sqrtTamTab, tab);
    }

    next:           /** NEXT **/
    system("pause");
    limpiarPantalla();
}

void asistencia()
{
    limpiarPantalla();
    printf(S_MAGENTA);
    printf("El juego es por turnos, cada jugador debe digitar la coordenada de alguna casilla\n");
    printf("en su turno, y si era una mina, perderá una vida.\n\n");
    printf(S_YELLOW);
    printf("Cada jugador cuenta con solo dos vidas, entonces el juego acabará si alguien pierde todas\n");
    printf("sus vidas o si solo quedan minas en las casillas restantes por descubrir.\n\n");
    printf(S_GREEN);
    printf("Al descubrir casillas, una \"x\" indica una mina, y cualquier otro número\n");
    printf("indica la cantidad de minas que hay al lado de esa casilla.\n\n");
    printf(S_RED);
    printf("Puedes cambiar el tamaño del tablero modificando el valor de esa macro :]\n\n");
    printf(DEFAULT_COLOR);
    system("pause");
    limpiarPantalla();
}

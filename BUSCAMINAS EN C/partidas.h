// Colores para imprimir en pantalla
#define DEFAULT_COLOR "\033[0m"

#define S_RED "\033[1;31m"
#define S_GREEN "\033[1;32m"
#define S_YELLOW "\033[01;33m"
#define S_BLUE "\033[1;34m"
#define S_MAGENTA "\033[1;35m"

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"

// Nombre del archivo de puntajes
#define PUNTAJES "scores.dat"
// Nombre del archivo para reproducir las partidas
#define PARTIDAS "partidas.dat"

/* Una variable de tipo Tablero consta de una parte visible y otra que no lo es */
typedef struct{
    char item;
    int mina;
} Tablero;

/* Estructura con los campos necesarios de un jugador */
typedef struct{
    char nombre[20];
    int vidas;
    int puntaje;
} Jugador;

/* Declaración de una cola para guardar las partidas y poder reproducirlas */
typedef struct n{
    char jugada[4];
    struct n *sig;
} Nodo;

extern int tam;
/* Funciones necesarias para empezar una partida */
void limpiarPantalla();
void regisJugadores(Jugador *j1, Jugador *j2);
void crearTablero(Tablero tab[tam][tam]);
void imprimir(int t, Tablero tab[t][t]);
int jugada(int turno, Jugador *j1, Jugador *j2, Tablero tab[tam][tam]);

/* Funciones para acceder a los archivos creados y la de asistencia */
void mejores();
void partidasJugadas();
void asistencia();

/*
    Lista de funciones static en partidas.c

    1. static void insCola(char jugada[4]);
    2. static int descubrir(int x, int y, int t, Tablero tab[t][t], char jug[4], int func);
    3. static int cantNodos();
    4. static void finalPartida(Jugador *j1, Jugador *j2, int jugadas, Tablero tab[tam][tam]);
    5. static int hayMinas(Tablero tab[tam][tam]);
    6. static void reproducir(char *nm1, char *nm2, int t, Tablero tab[t][t]);

    En ese orden se encuentran, y sus descrpciones están arriba de la defición de cada una
*/

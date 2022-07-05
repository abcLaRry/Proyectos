/*
    Implementación de los nodos de cada árbol
*/
typedef struct _nodo{
    char *palabra;
    int cantidad;
    struct _nodo *der, *izq;
} Nodo;

typedef Nodo* pNodo;
/*
    Implementación del tipo de dato DATO, del cual se hará un vector de tipo
    DATO de 26 elementos (uno para cada letra del abecedario)
*/
typedef struct{
    char letra;
    pNodo raiz;
    int cantPalabras;
} DATO;

/*
    Cabeceras de funciones para las opciones del menú, no tienen parámetros salvo una
    de ellas ya que ejerce 2 funcionalidades, los detalles los puede leer en el archivo "arboles.c"
*/
void limpiarPant();
void inicializar();
void agregar_eliminar(int funcion);
void mostrarPalabras();
void infoPalabra();
void infoPalabras();
void fin();
/*
    Lista de funciones static de "arboles.c":

    1. static void insertar(char *pal, int letra);
    2. static pNodo buscar(char *pal, int letra);
    3. static pNodo buscarPadre(pNodo hijo, int letra);
    4. static pNodo hNoNull(pNodo evaluar);
    5. static void eliminar(pNodo padre, pNodo del, int letra);
    6. static void eliminar(pNodo padre, pNodo del, int letra);
    7. static int buscarPalabra(char *palabra);
    8. static void imprimirCantidades(pNodo raiz, int repetida);
    9. static void liberar(pNodo raiz);
*/

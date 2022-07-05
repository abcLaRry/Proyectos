#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "arboles.h"

DATO diccionario[26];

void limpiarPant()
{
    setlocale(LC_ALL, "spanish");
    system("cls");
}

void inicializar(int ini)
{
    if(ini)
    {
        int i, auxLetras=97;

        for(i=0; i<26; ++i)
        {
            diccionario[i].letra = (char)auxLetras;
            diccionario[i].raiz = NULL;
            diccionario[i].cantPalabras = 0;
            auxLetras++;
        }
        printf("\nDiccionario inicializado con éxito.\n\n");
    }
    else printf("\n¡El diccionario ya ha sido inicializado!\n\n");

    system("pause");
}
/*
  Inserta palabras en su respectivo arbol, así como de inicializar todos sus campos:
  ->palabra, ->der, ->izq, ->cantidad. Cantidad es un miembro auxiliar para saber cuántas veces
  se repite una palabra en un árbol. Los nodos iguales siempre se guardan a la derecha, y cada
  subNodo derecho igual a su padre su miembro "->cantidad" es una unidad menor al de su padre,
  por lo que, si se elimina un Nodo, se elimina al de menor nivel, quedando su descendencia derecha
  guardando la cantidad nueva de veces que se repite una palabra.
*/
static void insertar(char *pal, int letra)
{
    pNodo nuevo = malloc(sizeof(Nodo)+sizeof(char)*strlen(pal)+1); // Reservamos para el nodo y para
    nuevo->palabra = (void*)nuevo + sizeof(Nodo);                  // la palabra
    strcpy(nuevo->palabra, pal);
    nuevo->der=nuevo->izq=NULL;
    nuevo->cantidad=1;

    if(!diccionario[letra].raiz)
        diccionario[letra].raiz = nuevo;
    else
    {
        pNodo reco=diccionario[letra].raiz, ant=NULL;
        while(reco!=NULL)
        {
            ant = reco;
            if(!strcmp(nuevo->palabra, reco->palabra))
                reco->cantidad++;
            if(strcmp(nuevo->palabra, reco->palabra)<0)
                reco=reco->izq;
            else
                reco=reco->der;
        }
        if(strcmp(nuevo->palabra, ant->palabra)<0)
            ant->izq = nuevo;
        else
            ant->der = nuevo;
    }
    diccionario[letra].cantPalabras++;
}
/*Busca un nodo y lo retorna*/
static pNodo buscar(char *pal, int letra)
{
    pNodo reco=diccionario[letra].raiz;

    while(reco!=NULL)
    {
        if(strcmp(pal, reco->palabra)<0)
            reco=reco->izq;
        else if(strcmp(pal, reco->palabra)>0)
            reco=reco->der;
        else return reco;
    }
    reco=NULL;
    return reco;
}
/*Recibe un nodo y el arbol al que pertenece y retorna su padre, si no lo tiene, retorna NULL*/
static pNodo buscarPadre(pNodo hijo, int letra)
{
    pNodo reco = diccionario[letra].raiz;

    while(reco!=NULL)
    {
        if(reco->der==hijo || reco->izq==hijo)
            return reco;
        else if(strcmp(hijo->palabra, reco->palabra)<0)
                reco=reco->izq;
        else reco = reco->der;
    }
    reco=NULL;
    return reco;
}
/*Retorna el hijo no NULL de un nodo "evaluar"*/
static pNodo hNoNull(pNodo evaluar)
{
    if(evaluar->der!=NULL)
        return evaluar->der;
    else return evaluar->izq;
}
/*Elimina palabras del diccionario, recibe el nodo que se desea eliminar así como a su padre y el
  árbol en el que se encuentran*/
static void eliminar(pNodo padre, pNodo del, int letra)
{
    if(!del->der && !del->izq) /** Es hoja **/
    {
        if(padre!=NULL) // 1A - No es la raíz
            if(padre->izq==del) // Su hijo a eliminar está a la izquierda
                padre->izq=NULL;
            else padre->der=NULL; // Entonces está a la derecha
        else diccionario[letra].raiz=NULL; // 1B - Es la raíz
        free(del);
    }
    else if(!del->der||!del->izq)  /** Tiene un solo hijo **/
    {
        pNodo herencia = hNoNull(del);
        if(padre!=NULL) // 2A No es la raíz

            if(padre->izq==del) //Su hijo a eliminar está a la derecha
                padre->izq=herencia;
            else padre->der=herencia; // Entonces está a la izquierda
        else
            diccionario[letra].raiz=herencia; // 2B - Es la raíz
        free(del);
    }
    else /** Tiene dos hijos **/
    {
        pNodo reco = del->izq;
        while(reco->der!=NULL)
            reco=reco->der;

        padre=buscarPadre(reco, letra); // Como nos movimos, debemos actualizar a su padre

        pNodo herencia=reco->izq; // Decimos que 'izq' porque reco es el nodo más a la derecha
                                  // por lo tanto no tiene más hijos derechos
        if(strcmp(reco->palabra, padre->palabra)<0)
            padre->izq=herencia;    // 3A - Su hijo izquierdo no tiene descendencia derecha
        else padre->der=herencia;   // 3B - Su hijo izquierdo sí tiene descendencia derecha

        strcpy(del->palabra, reco->palabra);
        free(reco); // Liberamos a reco, porque en realidad reubicamos el nodo que se buscaba eliminar
    }
    diccionario[letra].cantPalabras--;
}
/*Si su parámetro "funcion" vale 1, inserta palabras, si vale 0 las elimina*/
void agregar_eliminar(int funcion)
{
    char pal[100], s_n;
    int i;
    otra:
    printf("Digite a continuación la palabra:\n> ");
    scanf("%s", pal); fflush(stdin);

    for(i=0; i<strlen(pal); ++i)
        pal[i] = tolower((unsigned char) pal[i]); // pasamos todo a minusculas

    if(pal[0]>=97 && pal[0]<=122)
    {
        for(i=0; i<26; ++i)
            if((char)i+97== pal[0]) break;
        if(funcion)
        {
            insertar(pal, i);
            printf("¿Desea insertar otra palabra? (S/N): ");
            scanf("%c", &s_n);
            if(tolower(s_n)=='s') goto otra;
        }
        else
        {
            pNodo elim = buscar(pal, i); // Primero se busca el nodo

            if(elim!=NULL)  // Sí existe
            {
                pNodo padre = buscarPadre(elim, i); //Buscamos a su padre
                eliminar(padre, elim, i); // Lo borramos
                elim=padre=NULL;

                printf("¿Desea eliminar otra palabra? (S/N): ");
                scanf("%c", &s_n);
                if(tolower(s_n)=='s') goto otra;
            }
        }
    }
    else printf("Verifique haber ingresado la palabra correctamente.\n\n");
    system("pause");
}
/*Imprime un arbol InOrden*/
static void imprimir(pNodo raiz)
{
    if(raiz!=NULL)
    {
        imprimir(raiz->izq);
        printf("%s ", raiz->palabra);
        imprimir(raiz->der);
    }
}
/*Manda una a una las raices no NULL de los arboles del diccionario para imprimir su contenido con
  una función recursiva*/
void mostrarPalabras()
{
    for(int i=0; i<26; ++i)
    {
        if(diccionario[i].raiz==NULL)
            continue;
        imprimir(diccionario[i].raiz);
        printf("\n");
    }
    system("pause");
}

static int buscarPalabra(char *palabra)
{
    int i, cont=0;
    for(i=0; i<strlen(palabra); ++i)
        palabra[i] = tolower((unsigned char) palabra[i]); // Convertimos a minúscula

    for(i=0; i<26; ++i)
        if((char)i+97==diccionario[i].letra) break;  // Encontramos el árbol de su letra

    pNodo reco=diccionario[i].raiz;

    while(reco!=NULL)
    {
        if(strcmp(palabra, reco->palabra)<0)
            reco=reco->izq;
        else reco=reco->der;
        if(!strcmp(palabra, reco->palabra))
            cont++;
    }

    return cont;
}
/*Recibe la raiz de un arbol e imprime solo las palabras que se repiten y la cantidad de veces que se
  repiten respectivamente*/
static void imprimirCantidades(pNodo raiz, int repetida)
{
    if(raiz!=NULL)
    {
        imprimirCantidades(raiz->izq, repetida);
        if(raiz->cantidad>1&&repetida)
            printf("%s se repide %d veces.\n", raiz->palabra, raiz->cantidad);
        if(raiz->der!=NULL && !strcmp(raiz->palabra, raiz->der->palabra))
            repetida=0;
        else repetida=1;
        imprimirCantidades(raiz->der, repetida);
    }
}
/*Toda la información del diccionario se muestra si su parametro "funcion" vale 0, de lo contrario
  muestra solo la información de una palabra dada por el usuario*/
void infoPalabra(int funcion)
{
    if(funcion)
    {
        char pal[50];

        printf("Ingrese la palabra a buscar: ");
        scanf("%s", pal);

        printf("La palabra %s, aparece %d veces en el diccionario.\n\n", pal, buscarPalabra(pal));
    }
    else
    {
        int suma=0, i;
        for(i=0; i<26; ++i)
        {
            if(diccionario[i].cantPalabras>0)
                printf("Hay %d palabra%c con %c.\n", diccionario[i].cantPalabras, diccionario[i].cantPalabras>1? 's':' ',(char)i+97);
            suma+=diccionario[i].cantPalabras;
        }

        printf("Hay %d palabra%c en el diccionario.\n\n", suma, suma!=1?'s':' ');

        for(i=0; i<26; ++i)
            if(diccionario[i].raiz!=NULL) imprimirCantidades(diccionario[i].raiz, 1);
    }
    system("pause");
}
/*Libera la memoria de un arbol*/
static void liberar(pNodo raiz)
{
    if(raiz!=NULL)
    {
        liberar(raiz->izq);
        liberar(raiz->der);
        free(raiz);
    }
}
/*Llama una a una cada raiz de los 26 arboles para liberar la memoria*/
void fin()
{
    for(int i=0; i<26; ++i)
        liberar(diccionario[i].raiz);
}

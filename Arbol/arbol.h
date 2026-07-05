#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLA_DUP 1
#define SIN_MEM 2
#define TODO_BIEN 0

#define MINIMO(X,Y) ((X)<(Y)?(X):(Y))
#define MAXIMO(X,Y) ((X)>(Y)?(X):(Y))

typedef struct s_nodoA
{
    void *info;
    size_t tamInfo;
    struct s_nodoA *izq,
                   *der;
}t_nodoA;

typedef t_nodoA *t_arbol;

void crearArbol(t_arbol *pa);
void eliminarArbol(t_arbol *pa);
int insertarArbol(t_arbol *pa,
                         const void *d,
                         unsigned tam,
                         int (*cmp)(const void *, const void *));

unsigned alturaArbol(const t_arbol *pa);
t_nodoA ** mayorNodoArbol(const t_arbol * pa);
t_nodoA ** menorNodoArbol(const t_arbol * pa);

t_nodoA ** buscarEnArbol(const t_arbol * p, const void * d,
                                     int (*cmp)(const void*, const void *));

int recorrerArbolEnOrden(t_arbol *pa, void (*accion)(void *, void *), void* param);

int eliminarRaizArbol(t_arbol* pa);
int eliminarElemArbol(t_arbol *pa, void *d, unsigned tam, int (*cmp)(const void *, const void *));

#endif // ARBOL_H_INCLUDED

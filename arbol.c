#include "arbol.h"

void crearArbol(t_arbol *pa)
{
    *pa = NULL;
}

void eliminarArbol(t_arbol *pa)
{
    if(*pa)
    {
        eliminarArbol(&(*pa)->izq);
        eliminarArbol(&(*pa)->der);
        free((*pa)->info);
        free(*pa);
        *pa = NULL;
    }
}

int insertarArbol(t_arbol *pa, const void *d, unsigned tam, int (*cmp)(const void *, const void *))
{
    t_nodoA *nue;
    int rc;

    if (*pa) {
        rc = cmp(d, (*pa)->info);

        if (rc < 0)
            return insertarArbol(&(*pa)->izq, d, tam, cmp);
        if (rc > 0)
            return insertarArbol(&(*pa)->der, d, tam, cmp);

        // Si rc == 0, la clave ya existe
        return CLA_DUP;
    }

    // Si llegamos aca, encontramos el lugar. Reservamos memoria.
    nue = (t_nodoA *)malloc(sizeof(t_nodoA));
    if (!nue)
        return SIN_MEM;

    // Reservamos espacio para el dato gen�rico (el t_reg_indice)
    nue->info = malloc(tam);
    if (!nue->info) {
        free(nue);
        return SIN_MEM;
    }

    //Inicializamos el nodo
    nue->tamInfo = tam;
    memcpy(nue->info, d, tam); // Copiamos el contenido del registro
    nue->izq = NULL;
    nue->der = NULL;

    //Vinculamos al �rbol
    *pa = nue;

    return TODO_BIEN;
}

t_nodoA ** buscarEnArbol(const t_arbol * pa, const void * d,
                                     int (*cmp)(const void*, const void *))
{
    int rc;
    while(*pa && (rc=cmp(d, (*pa)->info)))
    {
        if(rc<0)
            pa = &(*pa)->izq;
        else
            pa = &(*pa)->der;
    }
    if(!*pa)
        return NULL;
    return (t_nodoA **)pa;
}

int recorrerArbolEnOrden(const t_arbol *pa, void (*accion)(void *, void *), void* param)
{
    if(!*pa)
        return 0;

    recorrerArbolEnOrden(&(*pa)->izq, accion, param);
    accion((*pa)->info, param);
    recorrerArbolEnOrden(&(*pa)->der, accion, param);

    return 1;
}

unsigned alturaArbol(const t_arbol *pa)
{
    if(!*pa)
        return 0;

    return MAXIMO(alturaArbol(&(*pa)->izq), alturaArbol(&(*pa)->der)) + 1;
}

t_nodoA ** mayorNodoArbol(const t_arbol * pa)
{
    if(!*pa)
        return NULL;
    while((*pa)->der)
        pa = &(*pa)->der;
    return (t_nodoA **) pa;
}


t_nodoA ** menorNodoArbol(const t_arbol * pa)
{
    if(!*pa)
        return NULL;
    while((*pa)->izq)
        pa = &(*pa)->izq;
    return (t_nodoA **) pa;
}


int eliminarRaizArbol(t_arbol* pa)
{
    t_nodoA **remp;
    t_nodoA *elim; //tnodoA* es como un tArbol

    if(!*pa)
        return 0;
    free((*pa)->info);

    if(!(*pa)->izq && !(*pa)->der)
    {
        free(*pa);
        *pa = NULL;
        return 1;
    }

    remp = alturaArbol(&(*pa)->izq) > alturaArbol(&(*pa)->der) ? //si la altura del arbol a la izq es mayot entonces busca el mayor del sub arbol izq, sino busco el mas chico del sub arbol derecho, en teoria eso encuentra el mas cercano al q busco eliminar
            mayorNodoArbol(&(*pa)->izq) :
            menorNodoArbol(&(*pa)->der);

    elim = *remp;
    (*pa)->info = elim->info; //pongo la info del q voy a eliminar en la raiz que va a reemplazar
    (*pa)->tamInfo = elim->tamInfo;

    *remp = elim->izq ? elim->izq : elim->der; //si el nodo de reemplazo tiene un hijo izq. Tuvo q haber sido pq tomamos el mayor del sub arbol izq, devuelve un puntero a el, sino devuelve un puntero al derecho, q tuvo q haber sido q se tomo el menor del subarbol der, y si no tiene devuelve null. es para dejar al hijo q corresponda o NULL en el lugar dodne solia estar el eliminado

    free(elim);

    return 1;
}

int buscarElemArbol(const t_arbol *p, void *d, unsigned tam,
                             int (*cmp)(const void *, const void *))
{
    if(!(p = buscarEnArbol(p,d,cmp)))
        return 0;
    memcpy(d, (*p)->info, MINIMO(tam, (*p)->tamInfo));
    return 1;
}

int eliminarElemArbol(t_arbol *pa, void *d, unsigned tam, int (*cmp)(const void *, const void *))
{
    if(!(pa = buscarEnArbol(pa, d, cmp))) //buscar en arbol devuelve tNodoA ** que es igual a tArbol*
        return 0;

    memcpy(d, (*pa)->info, MINIMO(tam, (*pa)->tamInfo));

    return eliminarRaizArbol(pa);
}

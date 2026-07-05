#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "tipos.h"
#include "validaciones.h"
#include "../Arbol/arbol.h"
#include "../Indice/indice.h"

char* extraer_campo(char **p);
int parsearCsvABinario(const char *path_csv);
int generarIndiceDesdeDat(const char *path_dat, t_indice *ind);
int compararDniIndice(const void *a, const void *b);
void imprimirIndice(const void* a, const void* b);

#endif // FUNCIONES_H_INCLUDED

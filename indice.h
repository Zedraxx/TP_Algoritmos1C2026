#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED

#include "tipos.h"
#include "arbol.h"

#define CLAVE_NO_ENCONTRADA 0
#define CLAVE_ENCONTRADA 1


void ind_crear(t_indice* ind, size_t tam_clave, int (*cmp)(const void*, const void*));
int ind_insertar(t_indice* ind, void *clave, unsigned nro_reg);
int ind_eliminar(t_indice* ind, void *clave, unsigned *nro_reg);
int ind_buscar(const t_indice* ind, void *clave, unsigned *nro_reg);
void grabar_nodo_long(const void *info, const void *param);
int ind_grabar(const t_indice* ind, const char* path);
void cargar_recursivo(t_indice *ind, FILE *fp, int inicio, int fin, size_t tamDato);
int ind_cargar(t_indice* ind, const char* path);
void vaciar_arbol_indice(t_arbol *pa);
void ind_vaciar(t_indice* ind);

#endif // INDICE_H_INCLUDED

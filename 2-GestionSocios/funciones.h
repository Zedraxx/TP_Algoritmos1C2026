#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "tipos.h"
#include "validaciones.h"
#include "../Arbol/arbol.h"
#include "../Indice/indice.h"

int compararDniIndice(const void *a, const void *b);
void imprimirIndice(const void* a, const void* b);

void accionListarSocio(const void *info, const void *param);
void menuAltaSocio(t_indice *ind, const char *path_dat);
void menuModificarSocio(t_indice *ind, const char *path_dat);
void menuBajaSocio(t_indice *ind, const char *path_dat);
void menuListarSocios(t_indice *ind, const char *path_dat);
void menuCompactarYReindexar(t_indice *ind, const char *path_dat, const char *path_idx);
t_fecha obtenerFechaActual();

#endif // FUNCIONES_H_INCLUDED

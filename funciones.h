#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "tipos.h"
#include "indice.h"
#include "validaciones.h"

char* extraer_campo(char **p);
int parsearCsvABinario(const char *path_csv);
int generarIndiceDesdeDat(const char *path_dat, t_indice *ind);
int compararDnis(const void *a, const void *b);

void accionListarSocio(const void *info, const void *param);
void menuAltaSocio(t_indice *ind, const char *path_dat);
void menuModificarSocio(t_indice *ind, const char *path_dat);
void menuBajaSocio(t_indice *ind, const char *path_dat);
void menuListarSocios(t_indice *ind, const char *path_dat);
void menuCompactarYReindexar(t_indice *ind, const char *path_dat, const char *path_idx);
t_fecha obtenerFechaActual();

#endif // FUNCIONES_H_INCLUDED

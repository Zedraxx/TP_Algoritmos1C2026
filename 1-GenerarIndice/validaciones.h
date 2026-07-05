#ifndef VALIDACIONES_H_INCLUDED
#define VALIDACIONES_H_INCLUDED

#include "tipos.h"
#include "../Arbol/arbol.h"
#include "../Indice/indice.h"

#define LIMINF 10000
#define LIMSUP 100000000
#define CANT_CATEGORIAS 6
#define LARGO_CATEGORIAS 10

#define validarDni(dni, limInf, limSup) (((dni) > limInf && (dni) < limSup) ? 0 : 1)
#define validarCaracter(caracter, opcion1, opcion2, opcion3) (((caracter) == opcion1 || (caracter) == opcion2 || (caracter) == opcion3) ? 0 : 1)

#define esBisiesto(anio) (((anio) % 4 == 0 && (anio) % 100 != 0 ) || (anio) % 400 == 0)

#define toUpper(caracter) (((caracter) >= 'a' && (caracter) <= 'z') ? ((caracter) - 32) : (caracter))
#define toLower(caracter) (((caracter) >= 'A' && (caracter) <= 'Z') ? ((caracter) + 32) : (caracter))
#define isSpace(caracter) (((caracter) == ' '  || (caracter) == '\t' || (caracter) == '\n') ? 1 : 0)

int validaciones(t_socio *socio, t_fecha* fProceso);

int esFechaValida(const t_fecha *);
int validarFechaNac(const t_fecha *, const t_fecha *);
int compararFechas(const t_fecha *, const t_fecha *);
void ingresarFecha(t_fecha *);
int validarCategoria(const char *);

#endif // VALIDACIONES_H_INCLUDED

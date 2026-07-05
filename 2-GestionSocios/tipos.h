#ifndef TIPOS_H_INCLUDED
#define TIPOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../Arbol/arbol.h"
#include "../Indice/indice.h"

typedef struct {
    int dia, mes, anio;
} t_fecha;

typedef struct {
    long dni;
    char ape[60];
    char nom[60];
    t_fecha fechaNac;
    char sexo;
    t_fecha fechaAf;
    char categoria[10];
    t_fecha fechaUCP;
    char estado;
    t_fecha fechaBaja;
} t_socio;

#endif // TIPOS_H_INCLUDED

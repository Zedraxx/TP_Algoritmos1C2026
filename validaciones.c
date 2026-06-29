#include "validaciones.h"

int validaciones(t_socio *socio, t_fecha* fProceso){
    int valor = 0;

    valor += validarDni(socio->dni, LIMINF, LIMSUP);

    //Valida la fecha de nacimiento
    valor += (esFechaValida(&socio->fechaNac)) ? 0 : 1;

    valor += validarCaracter(toUpper(socio->sexo), 'M', 'F', 'O');

    valor += validarCaracter(toUpper(socio->estado), 'A', 'B', 'I');

    //Valida la fecha de afiliacion. Si la fecha de afiliacion es valida (funcion es valida), y mayor a la del nacimiento, devuelve 0
    valor += (esFechaValida(&socio->fechaAf) && compararFechas(&socio->fechaAf, &socio->fechaNac) > 0) ? 0 : 1;

    valor += validarCategoria(socio->categoria);

    //Validaa la fecha de ultima cuota paga. Si la fecha de UCP es mayor a la de afiliacion devuelve 0
    valor += (compararFechas(&socio->fechaUCP, &socio->fechaAf)) > 0 ? 0 : 1;

    return valor;
}

/*******************************************    FUNCIONES DE FECHA     **********************************************/

//Devuelve 1 si la fecha es valida
int esFechaValida(const t_fecha *fecha){
    static const char dias[][12] = {
        { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };

    return fecha->anio > 1900 &&
            fecha->mes > 0 && fecha->mes <= 12 &&
            fecha->dia > 0 && fecha->dia <= dias[esBisiesto(fecha->anio)][fecha->mes - 1];
}


//Compara dos fechas y devuelve su diferencia
//Positivo si el primero es mayor
//Negativo si el segundo es mayor
int compararFechas(const t_fecha *fecha1, const t_fecha *fecha2){

    if(fecha1->anio != fecha2->anio)
        return fecha1->anio - fecha2->anio;
    if(fecha1->mes != fecha2->mes)
        return fecha1->mes - fecha2->mes;
    return fecha1->dia - fecha2->dia;

}

void ingresarFecha(t_fecha *fecha){

    do{
        printf("\tIngrese el dia: ");
        fflush(stdin);
        scanf("%d", &fecha->dia);

        printf("\tIngrese el mes: ");
        fflush(stdin);
        scanf("%d", &fecha->mes);

        printf("\tIngrese el anio: ");
        fflush(stdin);
        scanf("%d", &fecha->anio);

        if(!esFechaValida(fecha)){
            printf("\tLa fecha no es valida. Ingresela nuevamente: \n");
        }

    } while(!esFechaValida(fecha));
}


//Devuelve 0 si la cadena es igual a alguna cadena del vector categorias
int validarCategoria(const char *cadena){
    char categorias[CANT_CATEGORIAS][LARGO_CATEGORIAS] = {"MENOR", "CADETE", "ADULTO", "VITALICIO", "HONORARIO", "JUBILADO"};
    int i = 0;

    while(i < CANT_CATEGORIAS){

        if(strcmp(cadena, categorias[i]) == 0)
            return 0;

        i++;
    }

    return 1;
}

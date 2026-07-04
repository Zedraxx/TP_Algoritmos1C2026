#include "tipos.h"
#include "indice.h"
#include "funciones.h"
#include "validaciones.h"

#define ARCHIVO_ENTRADA "socios.csv"
#define ARCHIVO_DATOS "socios.dat"
#define ARCHIVO_INDICE "socios.idx"

// Primer aplicación: Generar el índice
/*
int main(int argc, char *argv[])
{
    t_indice ind;
    char* path;

    if(!argv[1]){
        printf("Falta agregar el Path en Program Arguments\n");
        return 1;
    }

    path = strcat(argv[1],ARCHIVO_ENTRADA);

    parsearCsvABinario(path);

    ind_crear(&ind,sizeof(long),compararDniIndice);

    generarIndiceDesdeDat(ARCHIVO_DATOS,&ind);

    ind_grabar(&ind,ARCHIVO_INDICE);

    ind_cargar(&ind, ARCHIVO_INDICE);

    printf("\nContenido del Indice\n");
    ind_recorrer(&ind,imprimirIndice,NULL);

    ind_vaciar(&ind);

    return 0;
}
*/


//Segunda aplicación: Gestión de Socios

int main()
{
    t_indice ind;
    char opcion;

    ind_crear(&ind, sizeof(long), compararDniIndice);

    // Intentamos cargar el índice; si no existe, se arranca con el índice vacío
    if (!ind_cargar(&ind, ARCHIVO_INDICE)) {
        printf("No se encontro un archivo de indices previo. Se creara uno nuevo al salir.\n");
    }

    do {
        printf("\n======= GESTION DE SOCIOS =======\n");
        printf("(A) Dar de Alta un Socio\n");
        printf("(M) Modificar Informacion de un Socio\n");
        printf("(B) Dar de Baja un Socio\n");
        printf("(L) Listar Socios Ordenados por DNI\n");
        printf("(C) Compactar y Reindexar Archivos\n");
        printf("(S) Salir\n");
        printf("Seleccione una opcion: ");

        fflush(stdin);
        scanf(" %c", &opcion);
        opcion = toUpper(opcion);

        switch(opcion) {
            case 'A':
                menuAltaSocio(&ind, ARCHIVO_DATOS);
                break;
            case 'M':
                menuModificarSocio(&ind, ARCHIVO_DATOS);
                break;
            case 'B':
                menuBajaSocio(&ind, ARCHIVO_DATOS);
                break;
            case 'L':
                menuListarSocios(&ind, ARCHIVO_DATOS);
                break;
            case 'C':
                menuCompactarYReindexar(&ind, ARCHIVO_DATOS, ARCHIVO_INDICE);
                break;
            case 'S':
                printf("Guardando cambios y saliendo...\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 'S');

    ind_grabar(&ind, ARCHIVO_INDICE);
    ind_vaciar(&ind);

    return 0;
}


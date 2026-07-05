#include "tipos.h"
#include "indice.h"
#include "funciones.h"
#include "validaciones.h"

#define ARCHIVO_ENTRADA "socios.csv"
#define ARCHIVO_DATOS "socios.dat"
#define ARCHIVO_INDICE "socios.idx"

int main(int argc, char *argv[])
{
    t_indice ind;
    char path_csv[512];//Buffer para el path
    char opcion;

    ind_crear(&ind, sizeof(long), compararDniIndice);

    // validamos si ya existe un indice, si existe es pq no es la primera ejecucion del programa y cargamos lo que se tenia previamente para no sobreescribir los cambios previos
    FILE* test_idx = fopen(ARCHIVO_INDICE, "rb");

    if (test_idx != NULL) {//Si el puntero no es null, entonces el .idx ya existe
        fclose(test_idx);
        printf("Se detecto un indice previo. Cargando...\n");
        ind_cargar(&ind, ARCHIVO_INDICE);
    } else {//sino es xq es la primera ejecucion del programa

        // validamos q hayan pasado el path
        if (argc < 2) {
            printf("Error: Falta agregar el Path en los argumentos\n");
            return 1;
        }

        // Armamos el path, snprintf permite escribir texto con formato directamente dentro de un buffer.
        // Es una version segura de sprintf, ya que evita el desbordamiento de memoria al limitar el num max de caracteres
        snprintf(path_csv, sizeof(path_csv), "%s%s", argv[1], ARCHIVO_ENTRADA);

        printf("Primera ejecucion. Procesando archivo historico: %s\n", path_csv);

        //generamos archivos
        parsearCsvABinario(path_csv);
        generarIndiceDesdeDat(ARCHIVO_DATOS, &ind);
    }

    do {
        printf("\n======= GESTION DE SOCIOS =======\n");
        printf("(A) Dar de Alta un Socio\n");
        printf("(M) Modificar Informacion de un Socio\n");
        printf("(B) Dar de Baja un Socio\n");
        printf("(L) Listar Socios Ordenados por DNI\n");
        printf("(C) Compactar y Reindexar Archivos\n");
        printf("(S) Salir\n");
        printf("\nSeleccione una opcion: ");

        scanf(" %c", &opcion);
        opcion = toupper(opcion);

        switch(opcion) {
            case 'A':
                system("cls");
                menuAltaSocio(&ind, ARCHIVO_DATOS);
                break;
            case 'M':
                system("cls");
                menuModificarSocio(&ind, ARCHIVO_DATOS);
                break;
            case 'B':
                system("cls");
                menuBajaSocio(&ind, ARCHIVO_DATOS);
                break;
            case 'L':
                system("cls");
                menuListarSocios(&ind, ARCHIVO_DATOS);
                break;
            case 'C':
                system("cls");
                menuCompactarYReindexar(&ind, ARCHIVO_DATOS, ARCHIVO_INDICE);
                break;
            case 'S':
                system("cls");
                printf("\nGuardando cambios y liberando memoria...\n");
                break;
            default:
                system("cls");
                printf("\nOpcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 'S');

    // cuando sale grabamos el indice al .idx y liberamos la memoria del indice.
    ind_grabar(&ind, ARCHIVO_INDICE);
    ind_vaciar(&ind);

    return 0;
}

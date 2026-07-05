#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tipos.h"
#include "indice.h"
#include "funciones.h"
#include "validaciones.h"

#define ARCHIVO_DATOS "socios.dat"
#define ARCHIVO_INDICE "socios.idx"

int main(void)
{
    t_indice ind;
    char path_csv[512];
    char path_usuario[250];
    int opcion_app;
    char opcion_menu;

    // Inicializamos la estructura del índice en memoria
    ind_crear(&ind, sizeof(long), compararDniIndice);

    do {
        printf("\n=========================================\n");
        printf("       SISTEMA DE GESTION DE SOCIOS      \n");
        printf("=========================================\n");
        printf("1. Primera Aplicacion: Generar Indice (desde CSV)\n");
        printf("2. Segunda Aplicacion: Gestion de Socios (Menu Interno)\n");
        printf("3. Salir del Programa\n");
        printf("=========================================\n");
        printf("Seleccione una aplicacion: ");

        if (scanf("%d", &opcion_app) != 1) {
            printf("Entrada invalida.\n");
            while (getchar() != '\n'); // Limpiar buffer
            continue;
        }

        switch (opcion_app) {
            case 1:
                printf("\n--- PRIMERA APLICACIÓN: GENERAR ÍNDICE ---\n");
                printf("Ingrese el path (ruta de acceso) de la carpeta del archivo socios.csv: ");
                scanf(" %511s", path_usuario);

                // Armamos la ruta completa combinando la carpeta ingresada con el nombre del archivo
                snprintf(path_csv, sizeof(path_csv), "%s/%s", path_usuario, "socios.csv");

                printf("Procesando archivo historico: %s...\n", path_csv);

                // Procesamos y generamos socios.dat y el índice en memoria
                if (parsearCsvABinario(path_csv) != 0) { // Asumiendo que retorna 0 si fue exitoso
                    generarIndiceDesdeDat(ARCHIVO_DATOS, &ind);
                    // Guardamos inmediatamente el archivo .idx como pide la consigna 1
                    ind_grabar(&ind, ARCHIVO_INDICE);
                    printf("¡Indice generado y guardado con exito en %s!\n", ARCHIVO_INDICE);
                } else {
                    printf("Error al procesar el archivo CSV. Verifique la ruta.\n");
                }
                break;

            case 2:
                printf("\n--- SEGUNDA APLICACIÓN: GESTIÓN DE SOCIOS ---\n");

                // Antes de abrir el menú, intentamos cargar el índice existente para no perder datos
                FILE* test_idx = fopen(ARCHIVO_INDICE, "rb");
                if (test_idx != NULL) {
                    fclose(test_idx);
                    printf("Cargando indice previo (%s)...\n", ARCHIVO_INDICE);
                    ind_cargar(&ind, ARCHIVO_INDICE);
                } else {
                    printf("Advertencia: No se encontro el archivo de indices '%s'.\n", ARCHIVO_INDICE);
                    printf("Debe ejecutar la opcion 1 primero o asegurarse de que el archivo exista.\n");
                    break;
                }

                // Submenú interactivo de gestión
                do {
                    printf("\n======= GESTION DE SOCIOS =======\n");
                    printf("(A) Dar de Alta un Socio\n");
                    printf("(M) Modificar Informacion de un Socio\n");
                    printf("(B) Dar de Baja un Socio\n");
                    printf("(L) Listar Socios Ordenados por DNI\n");
                    printf("(C) Compactar y Reindexar Archivos\n");
                    printf("(S) Volver al Menu Principal\n");
                    printf("\nSeleccione una opcion: ");

                    scanf(" %c", &opcion_menu);
                    opcion_menu = toupper(opcion_menu);

                    switch(opcion_menu) {
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
                            printf("\nGuardando cambios de Gestion en el archivo de indices...\n");
                            break;
                        default:
                            printf("\nOpcion invalida. Intente de nuevo.\n");
                    }
                } while (opcion_menu != 'S');

                // Al salir de la App 2, respaldamos los cambios en el archivo físico
                ind_grabar(&ind, ARCHIVO_INDICE);
                break;

            case 3:
                printf("\nSaliendo del programa. Liberando recursos...\n");
                break;

            default:
                printf("\nOpcion de aplicacion no valida.\n");
        }

    } while (opcion_app != 3);

    // Liberamos la memoria dinámica de la estructura del índice antes de terminar el proceso
    ind_vaciar(&ind);

    return 0;
}

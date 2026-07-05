#include "../Arbol/arbol.h"
#include "../Indice/indice.h"

#include "tipos.h"
#include "funciones.h"
#include "validaciones.h"

#define ARCHIVO_ENTRADA "socios.csv"
#define ARCHIVO_DATOS "socios.dat"
#define ARCHIVO_INDICE "socios.idx"
int main()
{
    t_indice ind;
    char opcion;

    // 1. Inicializar la estructura del índice en memoria
    ind_crear(&ind, sizeof(long), compararDniIndice);

    // 2. Cargar el índice previamente generado
    FILE* test_idx = fopen(ARCHIVO_INDICE, "rb");
    if (test_idx != NULL) {
        fclose(test_idx);
        printf("Cargando índice existente...\n");
        ind_cargar(&ind, ARCHIVO_INDICE);
    } else {
        printf("No se detectó un archivo de índice previo (%s).\n", ARCHIVO_INDICE);
        printf("Se creará uno nuevo al salir si se realizan operaciones.\n");
    }

    // 3. Bucle del menú interactivo de gestión
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
                printf("\nGuardando cambios y liberando memoria...\n");
                break;
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 'S');

    // 4. Finalización requerida: Grabar los índices en disco y liberar la memoria
    ind_grabar(&ind, ARCHIVO_INDICE);
    ind_vaciar(&ind);

    printf("Programa finalizado correctamente.\n");
    return 0;
}

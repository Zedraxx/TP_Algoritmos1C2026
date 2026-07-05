#include "../Arbol/arbol.h"
#include "../Indice/indice.h"

#include "tipos.h"
#include "funciones.h"

#define ARCHIVO_ENTRADA "socios.csv"
#define ARCHIVO_DATOS "socios.dat"
#define ARCHIVO_INDICE "socios.idx"

int main()
{
    t_indice ind;
    char path_base[256];
    char path_csv[512];

    // 1. Inicializar el índice en memoria
    ind_crear(&ind, sizeof(long), compararDniIndice);

    // 2. Solicitar al usuario el path por teclado
    printf("Ingrese la ruta de acceso (path) donde se encuentra el archivo: ");
    if (fgets(path_base, sizeof(path_base), stdin) != NULL) {
        path_base[strcspn(path_base, "\n")] = '\0';
    }

    // 3. Armar el path completo hacia el CSV
    int len = strlen(path_base);
    if (len > 0 && path_base[len - 1] != '/' && path_base[len - 1] != '\\') {
        snprintf(path_csv, sizeof(path_csv), "%s/%s", path_base, ARCHIVO_ENTRADA);
    } else {
        snprintf(path_csv, sizeof(path_csv), "%s%s", path_base, ARCHIVO_ENTRADA);
    }

    printf("Procesando archivo: %s\n", path_csv);

    // 4. Transformar datos a binario (socios.csv -> socios.dat)
    parsearCsvABinario(path_csv);

    // 5. Construir el índice en memoria leyendo el .dat generado
    generarIndiceDesdeDat(ARCHIVO_DATOS, &ind);

    // 6. Antes de finalizar, guardar el índice en "socios.idx" y liberar memoria
    printf("Guardando índice en %s...\n", ARCHIVO_INDICE);
    ind_grabar(&ind, ARCHIVO_INDICE);
    ind_vaciar(&ind);

    printf("Proceso finalizado con éxito.\n");
    return 0;
}

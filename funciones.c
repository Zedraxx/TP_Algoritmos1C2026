#include "funciones.h"

char* extraer_campo(char **p)
{
    char *inicio = *p;

    if (!inicio) return ""; // si ya es el final devuelve vacio

    char *coma = strchr(inicio, ','); // buscamos la primera coma

    if (coma) {
        *coma = '\0';       //reemplazamos la coma con un fin de cadena
        *p = coma + 1;      // Avanzamos
    } else {
        *p = NULL;          // no hay mas comas
    }

    return inicio;
}

int parsearCsvABinario(const char *path_csv)
{
    FILE *pfCsv = fopen(path_csv, "rt");
    if (!pfCsv) {
        printf("Error: No se pudo abrir el archivo %s\n", path_csv);
        return 0;
    }

    // Creamos el DAT
    FILE *pfDat = fopen("socios.dat", "wb");
    if (!pfDat) {
        printf("Error: No se pudo crear el archivo socios.dat\n");
        fclose(pfCsv);
        return 0;
    }

    char linea[512];
    unsigned nro_reg = 0;
    t_socio socio;

    // Leemos hasta final de arch
    while (fgets(linea, sizeof(linea), pfCsv)) {

        linea[strcspn(linea, "\n")] = '\0'; //buscamos el \n y lo reemp x un \0

        char *p = linea;
        char *campo;

        // 1. DNI
        campo = extraer_campo(&p);
        socio.dni = atol(campo); //el atol es para pasarlo de cadena a long

        // 2. Apellidos
        campo = extraer_campo(&p);
        strcpy(socio.ape, campo);

        // 3. Nombres
        campo = extraer_campo(&p);
        strcpy(socio.nom, campo);

        // 4. Fecha Nacimiento
        campo = extraer_campo(&p);
        sscanf(campo, "%d/%d/%d", &socio.fechaNac.dia, &socio.fechaNac.mes, &socio.fechaNac.anio);

        // 5. Sexo
        campo = extraer_campo(&p);
        socio.sexo = campo[0];

        // 6. Fecha de afiliacion
        campo = extraer_campo(&p);
        sscanf(campo, "%d/%d/%d", &socio.fechaAf.dia, &socio.fechaAf.mes, &socio.fechaAf.anio);

        // 7. Categoria
        campo = extraer_campo(&p);
        strcpy(socio.categoria, campo);

        // 8. Fecha de ult cuota paga
        campo = extraer_campo(&p);
        sscanf(campo, "%d/%d/%d", &socio.fechaUCP.dia, &socio.fechaUCP.mes, &socio.fechaUCP.anio);

        // 9. Estado
        campo = extraer_campo(&p);
        socio.estado = campo[0];

        // 10. Fecha de Baja
        campo = extraer_campo(&p);
        if (strlen(campo) > 0) {
            sscanf(campo, "%d/%d/%d", &socio.fechaBaja.dia, &socio.fechaBaja.mes, &socio.fechaBaja.anio);
        } else {
            socio.fechaBaja.dia = 0;
            socio.fechaBaja.mes = 0;
            socio.fechaBaja.anio = 0;
        }

        fwrite(&socio, sizeof(t_socio), 1, pfDat);
        nro_reg++;
    }

    printf("Se procesaron y guardaron %u registros exitosamente.\n", nro_reg);

    fclose(pfCsv);
    fclose(pfDat);
    return 1;
}

int generarIndiceDesdeDat(const char *path_dat, t_indice *ind)
{
    FILE *fp = fopen(path_dat, "rb");
    if (!fp) return 0;

    t_socio socio;
    unsigned nro_reg = 0;

    fread(&socio, sizeof(t_socio), 1, fp);
    while (!feof(fp)) {
        ind_insertar(ind, &socio.dni, nro_reg);
        nro_reg++;
        fread(&socio, sizeof(t_socio), 1, fp);
    }

    fclose(fp);
    return 1;
}

int compararDniIndice(const void *a, const void *b)
{
    const t_reg_indice *regA = (const t_reg_indice *)a;
    const t_reg_indice *regB = (const t_reg_indice *)b;

    const long *dniA = (const long *)regA->clave;
    const long *dniB = (const long *)regB->clave;

    if (*dniA == *dniB) return 0;
    if (*dniA > *dniB) return 1;    // A es mayor
    return -1;                      // A es menor
}

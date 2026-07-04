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

void imprimirIndice(const void* a, const void* b){
    const t_reg_indice *regA = (const t_reg_indice *)a;

    printf("%ld : %u\n", *(const long*)regA->clave, regA->nro_reg);
}













t_fecha obtenerFechaActual() {
    t_fecha hoy;

    time_t tiempoSegundos = time(NULL);

    struct tm *infoTiempo = localtime(&tiempoSegundos);

    hoy.dia = infoTiempo->tm_mday;
    hoy.mes = infoTiempo->tm_mon + 1;
    hoy.anio = infoTiempo->tm_year + 1900;

    return hoy;
}

void accionListarSocio(const void *info, const void *param) {
    const t_reg_indice *reg = (const t_reg_indice *)info;
    FILE *fp = (FILE *)param;

    t_socio socio;
    fseek(fp, (long)reg->nro_reg * sizeof(t_socio), SEEK_SET);
    if (fread(&socio, sizeof(t_socio), 1, fp) == 1) {

        if (socio.estado != 'B') {
            printf("DNI: %-10ld | %-15s, %-15s | Cat: %-10s | Sexo: %c | Estado: %c\n",
                   socio.dni, socio.ape, socio.nom, socio.categoria, socio.sexo, socio.estado);
        }
    }
}

// (A) ALTA DE SOCIO

void menuAltaSocio(t_indice *ind, const char *path_dat) {
    long dni_ingresado;
    unsigned nro_reg_existente;
    t_socio nuevo_socio;
    t_fecha fProceso = obtenerFechaActual();

    printf("\n--- ALTA DE NUEVO SOCIO ---\n");
    printf("Ingrese DNI: ");
    scanf("%ld", &dni_ingresado);

    if (validarDni(dni_ingresado, LIMINF, LIMSUP) != 0) {
        printf("Error: El DNI ingresado no cumple con los rangos permitidos del club.\n");
        return;
    }

    FILE *fp = fopen(path_dat, "r+b");
    if (!fp) {
        fp = fopen(path_dat, "wb");
        if (!fp) return;
    }

    if (ind_buscar(ind, &dni_ingresado, &nro_reg_existente) == CLAVE_ENCONTRADA) {
        fseek(fp, (long)nro_reg_existente * sizeof(t_socio), SEEK_SET);
        fread(&nuevo_socio, sizeof(t_socio), 1, fp);

        if (nuevo_socio.estado != 'B') {
            printf("Error: El socio con DNI %ld ya existe y esta activo.\n", dni_ingresado);
            fclose(fp);
            return;
        }
    }

    nuevo_socio.dni = dni_ingresado;
    printf("Ingrese Apellido: ");
    fflush(stdin);
    scanf(" %[^\n]", nuevo_socio.ape);

    printf("Ingrese Nombre: ");
    fflush(stdin);
    scanf(" %[^\n]", nuevo_socio.nom);

    printf("Fecha de Nacimiento:\n");
    ingresarFecha(&nuevo_socio.fechaNac);

    printf("Ingrese Sexo (M/F/O): ");
    fflush(stdin);
    scanf(" %c", &nuevo_socio.sexo);

    printf("Fecha de Afiliacion:\n");
    ingresarFecha(&nuevo_socio.fechaAf);

    printf("Ingrese Categoria (MENOR/CADETE/ADULTO/VITALICIO/HONORARIO/JUBILADO): ");
    fflush(stdin);
    scanf(" %[^\n]", nuevo_socio.categoria);

    for(int i = 0; nuevo_socio.categoria[i]; i++) {
        nuevo_socio.categoria[i] = toUpper(nuevo_socio.categoria[i]);
    }

    printf("Fecha de Ultima Cuota Paga:\n");
    ingresarFecha(&nuevo_socio.fechaUCP);

    nuevo_socio.estado = 'A';
    nuevo_socio.fechaBaja.dia = 0; nuevo_socio.fechaBaja.mes = 0; nuevo_socio.fechaBaja.anio = 0;

    int errores = validaciones(&nuevo_socio, &fProceso);

    if (errores > 0) {
        printf("\n[ERROR] El registro contiene %d inconsistencias logicas estructurales (Fechas cruzadas, sexo o categoria incorrecta).\n", errores);
        printf("El alta ha sido rechazada. Reintente operacion.\n");
        fclose(fp);
        return;
    }

    fseek(fp, 0, SEEK_END);
    long tam_archivo = ftell(fp);
    unsigned nuevo_nro_reg = (unsigned)(tam_archivo / sizeof(t_socio));

    fwrite(&nuevo_socio, sizeof(t_socio), 1, fp);
    fclose(fp);

    ind_insertar(ind, &nuevo_socio.dni, nuevo_nro_reg);
    printf("Socio %ld dado de alta exitosamente en la posicion: %u.\n", nuevo_socio.dni, nuevo_nro_reg);
}


// (M) MODIFICAR SOCIO

void menuModificarSocio(t_indice *ind, const char *path_dat) {
   long dni_buscar;
    unsigned nro_reg;
    t_socio socio;

    printf("\n--- MODIFICAR SOCIO ---\n");
    printf("Ingrese el DNI del socio a modificar: ");
    scanf("%ld", &dni_buscar);

    if (ind_buscar(ind, &dni_buscar, &nro_reg) == CLAVE_NO_ENCONTRADA) {
        printf("Socio no encontrado.\n");
        return;
    }

    FILE *fp = fopen(path_dat, "r+b");
    if (!fp) return;

    fseek(fp, (long)nro_reg * sizeof(t_socio), SEEK_SET);
    fread(&socio, sizeof(t_socio), 1, fp);

    if (socio.estado == 'B') {
        printf("El socio se encuentra dado de baja.\n");
        fclose(fp);
        return;
    }

    int opMod;
    printf("Campos:\n1. Apellido\n2. Nombre\n3. Categoria\n4. Sexo\n5. Fecha U.C.P.\nSeleccione: ");
    scanf("%d", &opMod);

    switch(opMod) {
        case 1:
            printf("Nuevo Apellido: ");
            scanf(" %[^\n]", socio.ape);
            break;
        case 2:
            printf("Nuevo Nombre: ");
            scanf(" %[^\n]", socio.nom);
            break;
        case 3: {
            char catAux[10];
            printf("Nueva Categoria: ");
            scanf(" %[^\n]", catAux);
            for(int i=0; catAux[i]; i++) catAux[i] = toUpper(catAux[i]);

            if (validarCategoria(catAux) == 0) {
                strcpy(socio.categoria, catAux);
            } else {
                printf("Error: Categoria inexistente. No se altero el registro.\n");
                fclose(fp);
                return;
            }
            break;
        }
        case 4: {
            char sexoAux;
            printf("Nuevo Sexo (M/F/O): ");
            scanf(" %c", &sexoAux);
            sexoAux = toUpper(sexoAux);

            if (validarCaracter(sexoAux, 'M', 'F', 'O') == 0) {
                socio.sexo = sexoAux;
            } else {
                printf("Error: Operador de Sexo invalido.\n");
                fclose(fp);
                return;
            }
            break;
        }
        case 5: {
            t_fecha fUcpAux;
            printf("Nueva Fecha Ultima Cuota Paga:\n");
            ingresarFecha(&fUcpAux);

            // UCP debe ser posterior a la fecha de afiliacion guardada
            if (compararFechas(&fUcpAux, &socio.fechaAf) > 0) {
                socio.fechaUCP = fUcpAux;
            } else {
                printf("Error: La fecha de U.C.P. no puede ser anterior a la de afiliacion (%02d/%02d/%04d).\n",
                       socio.fechaAf.dia, socio.fechaAf.mes, socio.fechaAf.anio);
                fclose(fp);
                return;
            }
            break;
        }
        default:
            printf("Opción invalida.\n");
            fclose(fp);
            return;
    }

    fseek(fp, (long)nro_reg * sizeof(t_socio), SEEK_SET);
    fwrite(&socio, sizeof(t_socio), 1, fp);
    fclose(fp);
    printf("Registro actualizado con exito.\n");
}

// (B) BAJA DE SOCIO

void menuBajaSocio(t_indice *ind, const char *path_dat) {
    long dni_baja;
    unsigned nro_reg;
    t_socio socio;

    printf("\n--- BAJA DE SOCIO ---\n");
    printf("Ingrese el DNI del socio a dar de baja: ");
    scanf("%ld", &dni_baja);

    if (ind_buscar(ind, &dni_baja, &nro_reg) == CLAVE_NO_ENCONTRADA) {
        printf("El socio no existe en el sistema o ya se encuentra dado de baja.\n");
        return;
    }

    FILE *fp = fopen(path_dat, "r+b");
    if (!fp) return;

    fseek(fp, (long)nro_reg * sizeof(t_socio), SEEK_SET);
    fread(&socio, sizeof(t_socio), 1, fp);

    if (socio.estado == 'B') {
        printf("El socio ya se encontraba con estado de baja ('B').\n");
        fclose(fp);
        return;
    }

    socio.estado = 'B';
    socio.fechaBaja = obtenerFechaActual();

    fseek(fp, (long)nro_reg * sizeof(t_socio), SEEK_SET);
    fwrite(&socio, sizeof(t_socio), 1, fp);
    fclose(fp);

    unsigned reg_eliminado;
    ind_eliminar(ind, &dni_baja, &reg_eliminado);

    printf("Socio %ld dado de baja del archivo y removido con exito del indice.\n", dni_baja);
}

// (L) LISTAR SOCIOS

void menuListarSocios(t_indice *ind, const char *path_dat) {
    FILE *fp = fopen(path_dat, "rb");
    if (!fp) {
        printf("No se pudo leer el archivo de socios.\n");
        return;
    }

    printf("\n=== LISTADO DE SOCIOS EN ORDEN (DNI) ===\n");

    ind_recorrer(ind, accionListarSocio, fp);
    fclose(fp);
}

// (C) COMPACTAR Y REINDEXAR

void menuCompactarYReindexar(t_indice *ind, const char *path_dat, const char *path_idx) {
    FILE *fpOrg = fopen(path_dat, "rb");
    if (!fpOrg) return;

    FILE *fpTmp = fopen("socios.tmp", "wb");
    if (!fpTmp) {
        fclose(fpOrg);
        return;
    }

    t_socio socio;
    unsigned nuevo_nro_reg = 0;

    ind_vaciar(ind);

    while (fread(&socio, sizeof(t_socio), 1, fpOrg) == 1) {
        if (socio.estado != 'B') {
            fwrite(&socio, sizeof(t_socio), 1, fpTmp);
            ind_insertar(ind, &socio.dni, nuevo_nro_reg);
            nuevo_nro_reg++;
        }
    }

    fclose(fpOrg);
    fclose(fpTmp);

    // Reemplazamos el viejo archivo
    remove(path_dat);
    rename("socios.tmp", path_dat);

    ind_grabar(ind, path_idx);

    printf("Compactacion terminada de manera exitosa. Registros activos remanentes: %u.\n", nuevo_nro_reg);
}

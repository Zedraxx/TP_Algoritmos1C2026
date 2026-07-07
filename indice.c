#include "indice.h"

typedef struct {
    FILE *fp;
    size_t tam_clave;
} t_grabar_param;

static void grabar_nodo(const void *info, const void *param);
static void cargar_recursivo(t_indice *ind, FILE *fp, int inicio, int fin, size_t tamDato);
static void liberar_clave_nodo(void *info, void *param);

static int (*_cmp_usuario_actual)(const void*, const void*) = NULL;

static int comparar_registros_internos(const void *a, const void *b)
{
    const t_reg_indice *regA = (const t_reg_indice *)a;
    const t_reg_indice *regB = (const t_reg_indice *)b;

    return _cmp_usuario_actual(regA->clave, regB->clave);
}

void ind_crear(t_indice* ind, size_t tam_clave, int (*cmp)(const void*, const void*))
{
    crearArbol(&(ind->arbol));

    ind->tam_clave = tam_clave;
    ind->cmp = cmp;
}

int ind_insertar(t_indice* ind, void *clave, unsigned nro_reg)
{
    t_reg_indice nuevo_registro;

    nuevo_registro.clave = malloc(ind->tam_clave);
    if (!nuevo_registro.clave)
        return 0;

    memcpy(nuevo_registro.clave, clave, ind->tam_clave);
    nuevo_registro.nro_reg = nro_reg;

    _cmp_usuario_actual = ind->cmp;

    int resultado = insertarArbol(&(ind->arbol), &nuevo_registro, sizeof(t_reg_indice), comparar_registros_internos);

    if (resultado != TODO_BIEN) {
        free(nuevo_registro.clave);
        return 0;
    }

    return 1;
}

int ind_eliminar(t_indice* ind, void *clave, unsigned *nro_reg)
{
    t_reg_indice buscado;
    buscado.clave = clave;

    _cmp_usuario_actual = ind->cmp;

    if (!eliminarElemArbol(&(ind->arbol), &buscado, sizeof(t_reg_indice), comparar_registros_internos)) {
        return CLAVE_NO_ENCONTRADA;
    }

    *nro_reg = buscado.nro_reg;

    free(buscado.clave);

    return CLAVE_ENCONTRADA;
}

int ind_buscar(const t_indice* ind, void *clave, unsigned *nro_reg)
{
    t_reg_indice buscado;
    buscado.clave = clave;

    _cmp_usuario_actual = ind->cmp;

    if (!buscarElemArbol(&(ind->arbol), &buscado, sizeof(t_reg_indice), comparar_registros_internos)) {
        return CLAVE_NO_ENCONTRADA;
    }

    *nro_reg = buscado.nro_reg;
    return CLAVE_ENCONTRADA;
}

int ind_recorrer(const t_indice* ind, void(*accion)(const void*, const void*), void *param)
{
    if (!ind || !ind->arbol) return 0;

    return recorrerArbolEnOrden(&ind->arbol, (void (*)(void *, void *))accion, param);
}


// recibe info desglosa y escribe, es la accion q tendra el recorrer
static void grabar_nodo(const void *info, const void *param)
{
    const t_reg_indice *reg = (const t_reg_indice *)info;
    const t_grabar_param *p = (const t_grabar_param *)param;

    // Calculamos el tamaño total del registro que vamos a guardar
    size_t tam_total = p->tam_clave + sizeof(unsigned);

    char buffer[tam_total];

    memcpy(buffer, reg->clave, p->tam_clave);
    memcpy(buffer + p->tam_clave, &reg->nro_reg, sizeof(unsigned));

    fwrite(buffer, tam_total, 1, p->fp);
}

int ind_grabar(const t_indice* ind, const char* path)
{
    FILE *fp = fopen(path, "wb");
    if(!fp) return 0;

    t_grabar_param parametros;
    parametros.fp = fp;
    parametros.tam_clave = ind->tam_clave;

    int resultado = ind_recorrer(ind, grabar_nodo, &parametros);

    fclose(fp);
    return resultado;
}


static void cargar_recursivo(t_indice *ind, FILE *fp, int inicio, int fin, size_t tamDato)
{
if (inicio > fin)
        return;

    int medio = (inicio + fin) / 2;

    // Nos posicionamos en el registro del medio
    fseek(fp, (long)medio * tamDato, SEEK_SET);

    // 1. Reservamos memoria para leer el registro COMPLETO (clave + nro_reg)
    char *buffer_lectura = malloc(tamDato);
    unsigned nro_reg;

    if (buffer_lectura) {
        fread(buffer_lectura, tamDato, 1, fp);

        memcpy(&nro_reg, buffer_lectura + ind->tam_clave, sizeof(unsigned));

        ind_insertar(ind, buffer_lectura, nro_reg);

        free(buffer_lectura);
    }

    // Recursividad para las mitades
    cargar_recursivo(ind, fp, inicio, medio - 1, tamDato);
    cargar_recursivo(ind, fp, medio + 1, fin, tamDato);
}

int ind_cargar(t_indice* ind, const char* path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
        return 0;

    fseek(fp, 0, SEEK_END);//nos ponemos en el final
    long tam_archivo = ftell(fp);//obtenemos el tam
    size_t tamDato = ind->tam_clave + sizeof(unsigned);//sacamos el tamanio del registro, clave + nro_reg
    int cant_registros = (int)(tam_archivo / tamDato);//dividimos tam total x el tamDato y obtenemos cant reg

    if (cant_registros > 0) {
        cargar_recursivo(ind, fp, 0, cant_registros - 1, tamDato);
    }

    fclose(fp);
    return 1;
}

static void liberar_clave_nodo(void *info, void *param)
{
    t_reg_indice *reg = (t_reg_indice *)info;
    if (reg && reg->clave) {
        free(reg->clave);
        reg->clave = NULL;
    }
}

void ind_vaciar(t_indice* ind)
{
    if (!ind || !(ind->arbol)) return;

    // Recorremos el árbol usando su primitiva pública para liberar las 'clave' dinámicas
    recorrerArbolEnOrden(&(ind->arbol), liberar_clave_nodo, NULL);

    // dejamos que el árbol se destruya a sí mismo con su propia primitiva.
    eliminarArbol(&(ind->arbol));
}

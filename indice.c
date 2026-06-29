#include "indice.h"

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

    int resultado = insertarArbol(&(ind->arbol), &nuevo_registro, sizeof(t_reg_indice), ind->cmp);

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

    if (!eliminarElemArbol(&(ind->arbol), &buscado, sizeof(t_reg_indice), ind->cmp)) {
        return CLAVE_NO_ENCONTRADA; // 0 si la clave no existia en el arbol
    }

    *nro_reg = buscado.nro_reg;

    free(buscado.clave);

    return CLAVE_ENCONTRADA; //1 si la pudo eliminar
}

int ind_buscar(const t_indice* ind, void *clave, unsigned *nro_reg)
{
    t_reg_indice buscado;
    buscado.clave = clave;

    t_nodoA **nodo_hallado = buscarEnArbol(&(ind->arbol), &buscado, ind->cmp);

    if (nodo_hallado == NULL) {
        return CLAVE_NO_ENCONTRADA; //Clave no encontrada
    }

    // Desreferenciamos el doble puntero (*nodo_hallado) para llegar al nodo
    // desp agarramos su campo 'info' y lo casteamos a reg_indice
    t_reg_indice *registro_real = (t_reg_indice *)(*nodo_hallado)->info;

    *nro_reg = registro_real->nro_reg;

    return CLAVE_ENCONTRADA;
}

int ind_recorrer(const t_indice* ind, void(*accion)(const void*, const void*), void *param)
{
    if (!ind || !ind->arbol) return 0;

    return recorrerArbolEnOrden((t_arbol *)&ind->arbol, (void (*)(void *, void *))accion, param);//Los tuve q castear pq me daba problemas con la firma de las funciones, el enunciado pide q la firma de esta primitiva tenga const, y la de arbol q vimos en clase no, asi que para respetar la firma lo castie asi
}


// recibe info desglosa y escribe,es la accion q tendra el recorrer
void grabar_nodo_long(const void *info, const void *param)
{
    // info es el nodo, param el puntero al arch
    t_reg_indice *reg = (t_reg_indice *)info;
    FILE *fp = (FILE *)param;

    long *dni = (long *)reg->clave;

    fwrite(dni, sizeof(long), 1, fp);
    fwrite(&reg->nro_reg, sizeof(unsigned), 1, fp);
}

int ind_grabar(const t_indice* ind, const char* path)
{
    FILE *fp = fopen(path, "wb");
    if(!fp) return 0;

    int resultado = ind_recorrer((t_indice*)ind, grabar_nodo_long, fp);

    fclose(fp);
    return resultado;
}


void cargar_recursivo(t_indice *ind, FILE *fp, int inicio, int fin, size_t tamDato)
{
    if (inicio > fin)
        return;

    int medio = (inicio + fin) / 2;

    // Nos posicionamos en el registro del medio
    fseek(fp, (long)medio * tamDato, SEEK_SET);

    // Reservamos memoria temporal para leer la clave
    void *buffer_clave = malloc(ind->tam_clave);
    unsigned nro_reg;

    if (buffer_clave) {
        fread(buffer_clave, ind->tam_clave, 1, fp);
        fread(&nro_reg, sizeof(unsigned), 1, fp);

        // Insertamos en el indice lo leido
        ind_insertar(ind, buffer_clave, nro_reg);

        free(buffer_clave);
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

void vaciar_arbol_indice(t_arbol *pa)
{
    if (!*pa) return;

    // vaciamos todos los hijos primero
    vaciar_arbol_indice(&(*pa)->izq);
    vaciar_arbol_indice(&(*pa)->der);

    // ahora q no estan los hijos, vaciamos la raiz de todos los sub arbol y del arbol gral
    t_reg_indice *reg = (t_reg_indice *)(*pa)->info;

    if (reg) {
        if (reg->clave) {
            free(reg->clave);
        }
        free(reg); //primero libero la clave luego registro
    }

    // luego liberamos el nodo del arbol y lo seteamos a null
    free(*pa);
    *pa = NULL;
}

void ind_vaciar(t_indice* ind)
{
    if (!ind || !ind->arbol) return;

    vaciar_arbol_indice(&(ind->arbol));//No pongo en 0 el tamClave y el cmp en null por si se quiere vaciar el indice y cargarlo desde otro .idx sin volver a crearlo
}

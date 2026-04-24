#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//  Estructuras

typedef struct Tarea {
    int TareaID;       // Autoincremental desde 1000
    char *Descripcion;
    int Duracion;      // entre 10 y 100
} Tarea;

typedef struct Nodo {
    Tarea T;
    struct Nodo *Siguiente;
} Nodo;

//  Variables globales

Nodo *TareasPendientes = NULL;
Nodo *TareasRealizadas = NULL;
int contadorID = 1000;

//  Funciones auxiliares

// Crea un nuevo nodo con la tarea dada
Nodo *crearNodo(Tarea t) {
    Nodo *nuevo = (Nodo *) malloc(sizeof(Nodo));
    nuevo->T = t;
    nuevo->Siguiente = NULL;
    return nuevo;
}

// Agrega un nodo al comienzo de una lista
void agregar(Nodo **lista, Nodo *nuevo) {
    nuevo->Siguiente = *lista;
    *lista = nuevo;
}

//  1) Carga de tareas pendientes
void cargarTarea() {
    char buff[100];
    int duracion;
    char opcion;

    do {
        Tarea nueva;
        nueva.TareaID = contadorID++;

        printf("\n--- Nueva Tarea (ID: %d) ---\n", nueva.TareaID);

        printf("Ingrese descripcion: ");
        fgets(buff, sizeof(buff), stdin);
        buff[strcspn(buff, "\n")] = '\0';
        nueva.Descripcion = (char *) malloc(strlen(buff) + 1);
        strcpy(nueva.Descripcion, buff);

        do {
            printf("Ingrese duracion (10-100): ");
            scanf("%d", &duracion);
            getchar();
        } while (duracion < 10 || duracion > 100);
        nueva.Duracion = duracion;

        Nodo *nodo = crearNodo(nueva);
        agregar(&TareasPendientes, nodo);

        printf("Tarea agregada correctamente.\n");
        printf("\n¿Desea ingresar otra tarea? (s/n): ");
        scanf("%c", &opcion);
        getchar();

    } while (opcion == 's' || opcion == 'S');
}


//  2) Marcar tarea como realizada
void marcarRealizada() {
    if (TareasPendientes == NULL) {
        printf("\nNo hay tareas pendientes.\n");
        return;
    }

    int id;
    printf("\nIngrese el ID de la tarea a marcar como realizada: ");
    scanf("%d", &id);
    getchar();

    Nodo *actual = TareasPendientes;
    Nodo *anterior = NULL;

    while (actual != NULL) {
        if (actual->T.TareaID == id) {
            // Desconectar de la lista de pendientes
            if (anterior == NULL) {
                TareasPendientes = actual->Siguiente;
            } else {
                anterior->Siguiente = actual->Siguiente;
            }
            actual->Siguiente = NULL;

            // Agregar a la lista de realizadas
            agregar(&TareasRealizadas, actual);
            printf("Tarea %d movida a realizadas correctamente.\n", id);
            return;
        }
        anterior = actual;
        actual = actual->Siguiente;
    }

    printf("No se encontro ninguna tarea pendiente con ID %d.\n", id);
}


//  3) Listar tareas pendientes y realizadas
void mostrarLista(Nodo *lista, const char *titulo) {
    printf("\n========== %s ==========\n", titulo);
    if (lista == NULL) {
        printf("  (Sin tareas)\n");
        return;
    }
    Nodo *actual = lista;
    while (actual != NULL) {
        printf("  ID        : %d\n", actual->T.TareaID);
        printf("  Descripcion: %s\n", actual->T.Descripcion);
        printf("  Duracion  : %d min\n", actual->T.Duracion);
        printf("  ---\n");
        actual = actual->Siguiente;
    }
}

void listarTareas() {
    mostrarLista(TareasPendientes, "TAREAS PENDIENTES");
    mostrarLista(TareasRealizadas, "TAREAS REALIZADAS");
}


//  4) Buscar tarea por ID o palabra clave
void buscarEnLista(Nodo *lista, const char *estado, int id, const char *clave, int *encontrado) {
    Nodo *actual = lista;
    while (actual != NULL) {
        int coincideID    = (id != -1 && actual->T.TareaID == id);
        int coincideClave = (clave[0] != '\0' && strstr(actual->T.Descripcion, clave) != NULL);

        if (coincideID || coincideClave) {
            printf("\n  [%s]\n", estado);
            printf("  ID        : %d\n", actual->T.TareaID);
            printf("  Descripcion: %s\n", actual->T.Descripcion);
            printf("  Duracion  : %d min\n", actual->T.Duracion);
            printf("  ---\n");
            *encontrado = 1;
        }
        actual = actual->Siguiente;
    }
}

void buscarTarea() {
    char opcion;
    int id = -1;
    char clave[100] = "";

    printf("\nBuscar por:\n");
    printf("  1) ID\n");
    printf("  2) Palabra clave\n");
    printf("Opcion: ");
    scanf("%c", &opcion);
    getchar();

    if (opcion == '1') {
        printf("Ingrese el ID a buscar: ");
        scanf("%d", &id);
        getchar();
    } else if (opcion == '2') {
        printf("Ingrese la palabra clave: ");
        fgets(clave, sizeof(clave), stdin);
        clave[strcspn(clave, "\n")] = '\0';
    } else {
        printf("Opcion invalida.\n");
        return;
    }

    int encontrado = 0;
    printf("\n====== RESULTADOS DE BUSQUEDA ======\n");
    buscarEnLista(TareasPendientes, "PENDIENTE", id, clave, &encontrado);
    buscarEnLista(TareasRealizadas, "REALIZADA", id, clave, &encontrado);

    if (!encontrado) {
        printf("No se encontro ninguna tarea.\n");
    }
}


//  Liberar memoria
void liberarLista(Nodo *lista) {
    Nodo *actual = lista;
    while (actual != NULL) {
        Nodo *siguiente = actual->Siguiente;
        free(actual->T.Descripcion);
        free(actual);
        actual = siguiente;
    }
}



int main() {
    char opcion;
    // Menu principal
    do {
        printf("\n===== MODULO TO-DO =====\n");
        printf("1) Cargar tarea pendiente\n");
        printf("2) Marcar tarea como realizada\n");
        printf("3) Listar todas las tareas\n");
        printf("4) Buscar tarea por ID o palabra clave\n");
        printf("5) Salir\n");
        printf("Opcion: ");
        scanf("%c", &opcion);
        getchar();

        switch (opcion) {
            case '1': cargarTarea();    break;
            case '2': marcarRealizada(); break;
            case '3': listarTareas();   break;
            case '4': buscarTarea();    break;
            case '5': printf("Saliendo...\n"); break;
            default:  printf("Opcion invalida.\n");
        }

    } while (opcion != '5');

    liberarLista(TareasPendientes);
    liberarLista(TareasRealizadas);

    return 0;
}
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#define BARRA "-------------------------------------------------------"

typedef struct{
  char nombre[31];
  int prioridad;
  List *precedencia;
} Tarea;

pene

typedef struct{
  char prece[31];
} precedencia;

void menu(){
  puts(BARRA);
  printf("          TAREA CON PRIORIDAD          \n\n");
  printf("Selecione una de las opciones disponibles\n\n");
  printf("1. Agregar tarea\n");
  printf("2. Establecer precedencia entre tareas\n");
  printf("3. Mostrar tareas por hacer\n");
  printf("4. Marcar tarea como completada\n");
  printf("5. Deshacer última acción\n");
  printf("6. Cargar datos de tareas desde un archivo de texto\n\n");
  printf("--------------Para finalizar ingrese 0--------------\n");
}

//esta funcion mete la estrutura a la casilla i del arreglo
void meterArreglo(Tarea* *arreglo, Tarea* estructura, int i){
  arreglo[i] = estructura;
}

//esta funcion agrega la tarea a una list auxiliar
void agregarTarea(List* lista, Tarea* estructura){
  pushBack(lista, estructura);
}

// funcion que completa la tarea indicada, tambien recibe un puntero aux el cual guarda la eleccion del usuario si quiere borrar la tarea, este aux se usa mas adelante en la funcion de completarTarea de el arreglo
void completarTarea( List* lista, char nombre[31], int *aux){
  int numero;
  for ( Tarea *i = firstList(lista); i != NULL ; i = nextList(lista)){
    if( strcmp(i->nombre, nombre) == 0){
      if(i->precedencia != NULL){
        printf("¿Esta seguro que quiere eliminar la tarea?\n)");
        printf(" Ingrese 1 para elminar, si ingresa otro numero la tarea no sera eliminada\n");
        scanf("%i", &numero);
        if( numero == 1){
          (*aux) = 1;
          popCurrent(lista);
        }
        else{
          return;
        }
      }
    }
  }
}

//esta funcion busca si hay una lista con el nombre de la tarea buscada, retorna 1 si la encuentra en caso contrario retorna 0
int buscarEnLista( List* lista, char tareaBuscada[31]){
  for (Tarea* i = firstList(lista) ; i != NULL ; i = nextList(lista)){
    if ( strcmp( i->nombre, tareaBuscada) == 0){
      return 1;
    }
  }
  return 0;
}
// funcoin que establece las precedencias para la lista auxiliar
void establecerPrecedencia(List* lista, char tarea1[31], char tarea2[31]){
  if( buscarEnLista( lista, tarea1) == 1){
    for (Tarea* i = firstList(lista) ; i != NULL ; i = nextList(lista)){
      if( strcmp(i->nombre, tarea2) == 0){
        Tarea* j = firstList(i->precedencia);
        if ( j == NULL){
          i->precedencia = createList();
        }
        precedencia* p = malloc( sizeof(precedencia));
        strcpy( p->prece, tarea1);
        pushBack(i->precedencia, p);
      }
    } 
  }
}

// esta funcion se ocupa con el quicksort para ordenar el arreglo
int compararTareas(const void *pivote, const void *elemento) {
  Tarea * const *ptrPivote = (Tarea * const *)pivote;
  Tarea * const *ptrElemento = (Tarea * const *)elemento;
  
  if ((*ptrElemento)->prioridad < (*ptrPivote)->prioridad) return 1;
  return 0;
}
// esta funcion muestra el arreglo 
void mostrarArreglo(Tarea* *arreglo, int largo){
  for( int i = 0 ; i < largo   ; i++){
    if ( arreglo[i] == NULL){
      return;
    }
    int contador = 0;
    printf(" Tarea: %s ( Prioridad %i )", arreglo[i]->nombre, arreglo[i]->prioridad);
    if( firstList(arreglo[i]->precedencia) != NULL){
      for( char* j = firstList(arreglo[i]->precedencia) ; j != NULL ; j = nextList(arreglo[i]->precedencia)){
        if (contador == 0){
          printf(" Precedencias: %s ", j);
          contador++;
         }
        else{
          printf(" , %s", j);
        }
      }
    }
    printf("\n");
  }
}
// funcion que reserva memoria para el arreglo 
Tarea *reservarMemoria(Tarea* arreglo, int i){
  Tarea* local = NULL;
  local = realloc(arreglo, sizeof(Tarea) * ( i + 1));
  return local;
}


//funcion que indica la posicion del arreglo de un nombre buscado
int buscarIndiceEnArreglo(Tarea* *arreglo, int largo, char nombre[31]){
  for( int i = 0 ; i < largo ; i++){
    if( arreglo[i]->precedencia != NULL){
      for(char* a = firstList(arreglo[i]->precedencia) ; a != NULL ; a = nextList(arreglo[i]->precedencia)){
        if( strcmp(a, nombre) == 0){
          return i;
        }
      }
    }
  }
  return EXIT_FAILURE;
}

//esta funcion busca si la tarea pertenece a una precedencia de otra tarea
int pertenecePrecedencia(List* lista, char nombre[31]){
  for(Tarea* a = firstList(lista) ; a != NULL ; a = nextList(lista)){
    if( a->precedencia != NULL){
      for(char* b = firstList(a->precedencia) ; b != NULL ; b = nextList(a->precedencia)){
        if( strcmp(b, nombre) == 0){
          return 1;
        }
      }
    }
  }
  return 2;
}

// ocupando el auxiliar de la funcion completar tarea de la lista, si este es 1 se elimina la tarea ingresada, ademas se recibe el puntero del largo para disminuirlo y hacer un realloc , en el caso contrario la funcion retorna el arreglo sin modificar
Tarea* *quitarDelArreglo(Tarea* *arreglo,int *largo,  char nombre[31], int aux){
  if ( aux != 1){
    return arreglo;
  }
  for(int i = 0 ; i < *largo ; i++){
    if( strcmp(arreglo[i]->nombre, nombre) == 0){
      for( int j = i   ; j <  *largo - 1 ; j++){
        Tarea* aux = arreglo[j];
        arreglo[j] = arreglo[j+1];
        arreglo[j+1] = aux;
        
      }
      (*largo) = (*largo) - 1;
      arreglo = reservarMemoria( arreglo, *largo);
      return arreglo;
    }
  }
  return arreglo;
}

//esta funcion es una de 3, en esta funcion primero se verifica se la casilla i es precedencia de otra tarea, en caso de que lo sea , se ordena de forma que la casilla i quede detrar de la que es precedencia
void ordenarPrioridad(Tarea* *arreglo, int largo, List* lista){
  for( int i = 0 ; i < largo ; i++){
    if( pertenecePrecedencia(lista, arreglo[i]->nombre) == 1){
      int indice = buscarIndiceEnArreglo(arreglo, largo, arreglo[i]->nombre);
      if( i < indice){
        for( int j = i ; j < indice - 1 ; j++){
          Tarea* aux = arreglo[j];
          arreglo[j] = arreglo[j+1];
          arreglo[j+1] = aux;
        }
      }
      else{
        for( int j = indice  ; j > i + 1  ; j--){
          Tarea* aux = arreglo[j];
          arreglo[j] = arreglo[j-1];
          arreglo[j-1] = aux;
        }
      }
    }
  }
}



// esta funcion es la segunda para ordenar por prioridad, primero se verifica si la lista de precedencias de la casilla i es distinto de null, si es distinto de null se verifica si la casilla i + 1 pertenece a la lista de precedencia de la lista de la casilla i, si se cumple se cambian de lugar
void ordenarPrioridad2(Tarea* *arreglo, int largo, List* lista) {
  for (int i = 0; i < largo - 1; i++) {  // Ajustar la condición del bucle
    if (arreglo[i]->precedencia != NULL) {
      char* a = firstList(arreglo[i]->precedencia);
      if( arreglo[i+1]->prioridad != NULL){
        if (strcmp(a, arreglo[i+1]->nombre) == 0) {
          Tarea* aux = arreglo[i];
          arreglo[i] = arreglo[i+1];
          arreglo[i+1] = aux;
        }
      }

    }
  }
}






//esta es la ultima funcion para ordenar por prioridad, primero se verifica si la lista de precedencias de la casilla i es nula, si es nula se verifica si la lista de precedencias de la casilla i + 1 es distinto de null y la tarea de la casilla i esta dentro de la lista, si se cumple se ordena 
void ordenarPrioridad3(Tarea* *arreglo, int largo, List* lista) {
  for (int i = 0; i < largo - 1; i++) {
    if (arreglo[i]->precedencia == NULL) {
      if (arreglo[i+1]->precedencia != NULL && strcmp(firstList(arreglo[i+1]->precedencia), arreglo[i]->nombre) == 0) {
        for (int j = i; j > 0; j--) {
          Tarea* aux = arreglo[j];
          arreglo[j] = arreglo[j-1];
          arreglo[j-1] = aux;
        }
      }
    }
  }
}




int main ( ){
  List *lista = createList(); // se crea una lista auxiliar
  Tarea* arreglo = NULL; // se crea un vector dinamico que se utilizara para ordenar
  char nombre[31], nombre2[31];
  int numeroCont, numero, largoNombre, i = 0, aux = 0;

  while(true){

    menu();
    scanf("%i", &numeroCont);
    while( numeroCont > 9 || numeroCont < 0){
      printf(" tiene que ingresar un numero entre 0 y 9\n");
      scanf("%i", &numeroCont);
    }

    if (numeroCont == 0){
      printf(" Programa Finalizado Con Exito");
      return 0;
    }
    if (numeroCont == 1){ // funcion 1
      Tarea *estructura = malloc( sizeof(Tarea));
      printf("Ingrese el Nombre de la tarea\n");
      scanf(" %[^\n]", nombre);
      largoNombre = strlen(nombre);
      while (largoNombre > 31 || largoNombre < 1) {
        printf("Ingrese un nombre válido (hasta 30 caracteres)\n");
        scanf(" %[^\n]", nombre);
        largoNombre = strlen(nombre);
      }
      strcpy(estructura->nombre, nombre);
      
      printf("Ingrese su prioridad\n");
      scanf("%i", &numero);
      estructura->prioridad = numero;
      agregarTarea(lista, estructura);
      arreglo = reservarMemoria( arreglo, i);
      meterArreglo(arreglo, estructura, i);
      i++;
      qsort(arreglo, i , sizeof(Tarea *), compararTareas);
    }
    if (numeroCont == 2){ // funcion 2
      printf("Ingrese el Nombre de la tarea 1\n");
      scanf(" %[^\n]", nombre);
      largoNombre = strlen(nombre);
      while (largoNombre > 31 || largoNombre < 1) {
        printf("Ingrese un nombre válido (hasta 30 caracteres)\n");
        scanf(" %[^\n]", nombre);
        largoNombre = strlen(nombre);
      }
      printf("Ingrese el Nombre de la tarea2\n");
      scanf(" %[^\n]", nombre2);
      largoNombre = strlen(nombre2);
      while (largoNombre > 31 || largoNombre < 1) {
        printf("Ingrese un nombre válido (hasta 30 caracteres)\n");
        scanf(" %[^\n]", nombre);
        largoNombre = strlen(nombre2);
      }
      establecerPrecedencia(lista, nombre, nombre2);
      
      ordenarPrioridad(arreglo, i, lista);
      ordenarPrioridad3(arreglo, i, lista);
      ordenarPrioridad2(arreglo, i, lista);
      
      
    }
    if (numeroCont == 3){ // funcion 3
      mostrarArreglo( arreglo, i);
    }
    if (numeroCont == 4){ // funcion 4
      printf("Ingrese la tarea completada\n");
      scanf(" %[^\n]", nombre);
      largoNombre = strlen(nombre);
      while (largoNombre > 31 || largoNombre < 1) {
        printf("Ingrese un nombre válido (hasta 30 caracteres)\n");
        scanf(" %[^\n]", nombre);
        largoNombre = strlen(nombre);
      }
      completarTarea(lista, nombre, &aux);
      arreglo = quitarDelArreglo(arreglo, &i,  nombre, aux);
      qsort(arreglo, i , sizeof(Tarea *), compararTareas);
      ordenarPrioridad(arreglo, i, lista);
      ordenarPrioridad2(arreglo, i, lista);
      ordenarPrioridad3(arreglo, i, lista);
      // al quitar un elemento del arreglo se debe ordenar denuevo
    }
    if (numeroCont == 5){ // funcion 5

    }
    if (numeroCont == 6){ // funcion 6
      
    }
  }
}
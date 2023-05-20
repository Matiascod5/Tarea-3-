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

void meterArreglo(Tarea* *arreglo, Tarea* estructura, int i){
  arreglo[i] = estructura;
}

void agregarTarea(List* lista, Tarea* estructura){
  pushBack(lista, estructura);
}

Tarea *reservarMemoria(Tarea* arreglo, int i){
  Tarea* local = NULL;
  local = realloc(arreglo, sizeof(Tarea) * ( i + 1));
  return local;
}

int compararTareas(const void *pivote, const void *elemento) {
  Tarea * const *ptrPivote = (Tarea * const *)pivote;
  Tarea * const *ptrElemento = (Tarea * const *)elemento;
  
  if ((*ptrElemento)->prioridad < (*ptrPivote)->prioridad) return 1;
  return 0;
}

int buscarEnLista( List* lista, char tareaBuscada[31]){
  for (Tarea* i = firstList(lista) ; i != NULL ; i = nextList(lista)){
    if ( strcmp( i->nombre, tareaBuscada) == 0){
      return 1;
    }
  }
  return 0;
}

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
        printf("hola1");
        pushBack(i->precedencia, p);
      }
    } 
  }
}

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
    }
  }
}

void ordenarPrioridad2(Tarea* *arreglo, int largo, List* lista) {
  for (int i = 0; i < largo - 1; i++) {  // Ajustar la condición del bucle
    if (arreglo[i]->precedencia != NULL) {
      char* a = firstList(arreglo[i]->precedencia);
      if (strcmp(a, arreglo[i+1]->nombre) == 0) {
        Tarea* aux = arreglo[i];
        arreglo[i] = arreglo[i+1];
        arreglo[i+1] = aux;
      }
    }
  }
}

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


int main(){
  List *lista = createList();
  Tarea* arreglo = NULL;
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
      ordenarPrioridad2(arreglo, i, lista);
      ordenarPrioridad3(arreglo, i, lista);
    }
    if (numeroCont == 3){ // funcion 3
      /*tareasPorHacer( lista);
      printf("\n");
      mostrarArreglo( arreglo, i);*/
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
      /*completarTarea(lista, nombre, &aux);
      arreglo = quitarDelArreglo(arreglo, &i,  nombre, aux);
      qsort(arreglo, i , sizeof(Tarea *), compararTareas);
      ordenarPrioridad(arreglo, i, lista);
      ordenarPrioridad2(arreglo, i, lista);
      ordenarPrioridad3(arreglo, i, lista);*/
    }
    if (numeroCont == 5){ // funcion 5

    }
    if (numeroCont == 6){ // funcion 6
      
    }
  }
}
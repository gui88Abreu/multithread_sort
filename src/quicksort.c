#include "quicksort.h"

int divide(int v[], int p, int r){
  int c = v[p], i = p+1, j = r, t;
  while (i <= j) {
    if (v[i] <= c) ++i;
    else if (c < v[j]) --j; 
    else {
        t = v[i], v[i] = v[j], v[j] = t;
        ++i; --j;
    }
  }
  // agora i == j+1                 
  v[p] = v[j], v[j] = c;
  return j; 
}

void print_vector(int *numbers_vector, int length){
  for(int i =0; i < 60; i++) printf("-");
  printf("\n");
  for(int i = 0; i < length-1; i++){
    printf("%d ", numbers_vector[i]);
  }
  printf("%d\n", numbers_vector[length-1]);
  return;
}

int *read_numbers(int *l, int size){
  char c;
  int length = 0;
  int *numbers_vector = (int *)malloc(size*sizeof(int));
  
  do{
    if (length == size){
      size *= 2;
      numbers_vector = (int *)realloc(numbers_vector, size*sizeof(int));
      if (numbers_vector == NULL){
        printf("ERROR 404: DEU RUIM\n");
        exit(EXIT_FAILURE);
      }
    }
    scanf("%d", &numbers_vector[length]);
    c = getchar();
    length += 1;
  }while (c != '\n');

  *l = length;
  return numbers_vector;
}
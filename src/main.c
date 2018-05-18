/*
 * Esta tarefa consiste em receber um texto na entrada e contar 
 * quantos números primos existem nela utilizando multithreads.
 */

#include <stdio.h>
#include <stdlib.h> /* dinamic memory*/
#include <pthread.h> /* threads*/
#include "stack.h"

#define N_THREADS 4

typedef struct thread{
  tsk task_list;
  int *n;
}thrd;

void *thread_worker(void *); /* thread que executa o quicksort*/
int divide(int v[], int p, int r); /* divide vetor v*/
int read_numbers(int *, int);

pthread_mutex_t key; /*chave utilizada para travar as threads quando for necessario*/

int main() {
  pthread_t thread[N_THREADS];
  tsk task_list;
  thrd thread_arg[N_THREADS];
  int n, length, size = 8;

  /* vetor que guarda os numeros lidos e variavel que contem a quantidade de numeros primos lidos*/
  int *numbers_vector = (int *) malloc(8*sizeof(int));

  /* Le no maximo N_MAX numeros inteiros seguidos de um \n*/
  length = read_numbers(numbers_vector, size);

  task_list = create_task_list(numbers_vector);
  push_task(task_list, 0, length-1);

  n = length;
  for (int i = 0; i < N_THREADS; i++){
    thread_arg[i].task_list = task_list;
    thread_arg[i].n = &n;
    pthread_create(&(thread[i]), NULL, thread_worker, &thread_arg[i]);
  }

  for(int i = 0; i < length-1; i++){
    printf("%d ", numbers_vector[i]);
  }
  printf("%d\n", numbers_vector[length-1]);

  free(numbers_vector);
  remove_task_list(task_list);
  return 0;
}

int read_numbers(int *numbers_vector, int size){
  char c;
  int length = 0;
  
  do{
    if (length == size){
      size *= 2;
      numbers_vector = (int *)realloc(numbers_vector, size*sizeof(int));
    }
    scanf("%d", &numbers_vector[length]);
    c = getchar();
    length += 1;
  }while (c != '\n');

  return length;
}

void *thread_worker(void *arg){
  thrd *t_arg = (thrd*)(arg);
  int p, r, j;

  while(*t_arg->n > 0 ){
    while(is_empty(t_arg->task_list) && *t_arg->n > 0);
    if(*t_arg->n  <= 0) break;

    pthread_mutex_lock(&key);
    pop_task(t_arg->task_list, &p, &r);
    pthread_mutex_unlock(&key);

    j = divide(t_arg->task_list->vector, p, r);
    
    pthread_mutex_lock(&key);
    if (p < j-1) push_task(t_arg->task_list, p, j-1);
    else *(t_arg->n) -= 1;

    if (j+1 < r) push_task(t_arg->task_list, j+1, r);
    else *(t_arg->n) -= 1;
    pthread_mutex_unlock(&key);
  }

  return NULL;
}

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
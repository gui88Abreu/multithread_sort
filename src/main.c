/*
 * Esta tarefa consiste em receber um texto na entrada e contar 
 * quantos números primos existem nela utilizando multithreads.
 */

#include <stdio.h>
#include <stdlib.h> /* dinamic memory*/
#include <pthread.h> /* threads*/
#include <unistd.h>
#include "stack.h"
#include "quicksort.h"

#define N_THREADS 4

typedef struct thread{
  tsk task_list;
  int *n_working;
}thrd;

void *thread_worker(void *); /* thread que executa o quicksort*/

pthread_mutex_t key1, key2; /*chave utilizada para travar as threads quando for necessario*/

int main() {
  pthread_t thread[N_THREADS];
  tsk task_list;
  thrd thread_arg[N_THREADS];
  int n, length, size = 8;

  /* vetor que guarda os numeros lidos e variavel que contem a quantidade de numeros primos lidos*/
  int *numbers_vector;

  /* Le no maximo N_MAX numeros inteiros seguidos de um \n*/
  numbers_vector = read_numbers(&length, size);
  print_vector(numbers_vector, length);
  task_list = create_task_list(numbers_vector);
  push_task(task_list, 0, length-1);

  n = 0;
  for (int i = 0; i < N_THREADS; i++){
    thread_arg[i].task_list = task_list;
    thread_arg[i].n_working = &n;
    pthread_create(&(thread[i]), NULL, thread_worker, &thread_arg[i]);
  }

  for(int i = 0; i < N_THREADS; i++){
    pthread_join(thread[i], NULL);
  }

  print_vector(numbers_vector, length);
  free(numbers_vector);
  remove_task_list(task_list);
  return 0;
}

void *thread_worker(void *arg){
  thrd *t_arg = (thrd*)(arg);
  int p, r, j;

  while (*t_arg->n_working > 0 || !is_empty(t_arg->task_list)){
    pthread_mutex_lock(&key1);
    while (is_empty(t_arg->task_list) && *t_arg->n_working > 0);
    if (is_empty(t_arg->task_list)) break;

    t_arg->n_working += 1;
    pthread_mutex_lock(&key2);
    pop_task(t_arg->task_list, &p, &r);
    pthread_mutex_unlock(&key2);
    pthread_mutex_unlock(&key1);

    j = divide(t_arg->task_list->vector, p, r);
    
    pthread_mutex_lock(&key2);
    if (p < j-1) push_task(t_arg->task_list, p, j-1);
    if (j+1 < r) push_task(t_arg->task_list, j+1, r);
    t_arg->n_working -= 1;
    pthread_mutex_unlock(&key2);
  }

  return NULL;
}
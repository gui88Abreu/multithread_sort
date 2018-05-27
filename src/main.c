/*
 * This code implemts quicksort algorithm with threads
 */

#include <stdio.h>
#include <stdlib.h> /* dinamic memory*/
#include <pthread.h> /* threads*/
#include <unistd.h>

#include "stack.h" /* Contain functions that implement a stack of tasks*/
#include "quicksort.h" /*Contain function that implement quicksort algorithm*/

#define N_THREADS 4

typedef struct thread{
  tsk task_list;
  int *n_working;
}thrd;

void *thread_worker(void *); /* thread function*/

pthread_mutex_t key1, key2;

int main() {
  pthread_t thread[N_THREADS];
  tsk task_list;
  thrd thread_arg[N_THREADS];
  int n, length, size = 8;

  int *numbers_vector;

  /* get input*/
  numbers_vector = read_numbers(&length, size);
  
  /*Create a task list and put the first task on it*/
  task_list = create_task_list(numbers_vector);
  push_task(task_list, 0, length-1);

  /*Initiate Threads*/
  n = 0;
  for (int i = 0; i < N_THREADS; i++){
    thread_arg[i].task_list = task_list;
    thread_arg[i].n_working = &n;
    pthread_create(&(thread[i]), NULL, thread_worker, &thread_arg[i]);
  }

  /*Wait for the end of the threads*/
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

  /*If there are tasks on the list or active threads, so the seek keep going*/
  while (*t_arg->n_working > 0 || !is_empty(t_arg->task_list)){
    /*key1 controls n_working*/
    /*key2 controls push and pop on task_list*/

    /*Seek tasks*/
    pthread_mutex_lock(&key1);
    while (is_empty(t_arg->task_list) && *t_arg->n_working > 0);
    
    /*If the list is empty so there is no more threads working*/
    if (is_empty(t_arg->task_list)){
      pthread_mutex_unlock(&key1);  
      break;
    }
    
    /*Add one more working thread*/
    t_arg->n_working += 1;

    /*Get a task from the list*/
    pthread_mutex_lock(&key2);
    pop_task(t_arg->task_list, &p, &r);
    pthread_mutex_unlock(&key1);
    pthread_mutex_unlock(&key2);

    /*Execute quicksort algorithm*/
    j = divide(t_arg->task_list->vector, p, r);
    
    /*Put the next tasks on the list if there is more*/
    pthread_mutex_lock(&key2);
    if (p < j-1) push_task(t_arg->task_list, p, j-1);
    if (j+1 < r) push_task(t_arg->task_list, j+1, r);
    pthread_mutex_unlock(&key2);
    
    /*Subtract one working thread*/
    pthread_mutex_lock(&key1);
    t_arg->n_working -= 1;
    pthread_mutex_unlock(&key1);
  }

  return NULL;
}
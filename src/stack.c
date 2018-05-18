#include "stack.h"

tsk create_task_list(int *vector){
  tsk new_list = (tsk)malloc(sizeof(task));
  new_list->vector = vector;
  new_list->next = NULL;
  return new_list;
}

int is_empty(tsk list){
  if (list->next) return 0;
  return 1;
}

void push_task(tsk list, int p, int r){
  tsk new_task = create_task_list(list->vector);
  new_task->p = p;
  new_task->r = r;
  new_task->next = list->next;
  list->next = new_task;
  return;
}

void pop_task(tsk list, int *p, int *r){
  if (list->next == NULL)
    return;
  
  tsk task = list->next;
  list->next = task->next;

  if (p != NULL && r != NULL)
    *p = task->p, *r = task->r;

  free(task);
  return;
}

void remove_task_list(tsk list){
  while (!is_empty(list)) pop_task(list, NULL, NULL);
  //free(list);
  return;
}

void print_list(tsk list){
  while(list->next){
    list = list->next;
    printf("(%d, %d), ", list->p, list->r);
  }
  printf("\n");
}
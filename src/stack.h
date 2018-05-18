#ifndef _STACK_H
#define _STACK_H

#include <stdlib.h>
#include <stdio.h>

typedef struct task{
  int p;
  int r;
  int *vector;
  struct task *next;
}task, *tsk;

tsk create_task_list(int *);
void push_task(tsk list, int, int);
void pop_task(tsk list, int *, int *);
void remove_task_list(tsk list);
int is_empty(tsk list);
void print_list(tsk list);

#endif // !_STACK_H
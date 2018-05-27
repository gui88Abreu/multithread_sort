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

tsk create_task_list(int *); /*Create a list*/
void push_task(tsk list, int, int); /*Put a task on top of the list*/
void pop_task(tsk list, int *p, int *r); /*Get the task on top of the list and remove it*/
void remove_task_list(tsk list); /*Remove all task on the list and remove list from the memory*/
int is_empty(tsk list); /*Check whether the list is empty. return 1 for True and 0 otherwise*/
void print_list(tsk list); /*Print all tasks on the list*/

#endif // !_STACK_H
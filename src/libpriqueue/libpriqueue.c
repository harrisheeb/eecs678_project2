/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"


/**
  Initializes the priqueue_t data structure.
  
  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
  q->front = q->rear = -1;
  q->comparer = comparer;
  q->size = 100;
  q->m_q = malloc(100*sizeof(void*));
}


/**
  Insert the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
  if(q->rear >= q->size - 1){
    resize(q);
  }
  if((q->front == -1) && (q->rear == -1)){
    q->front++;
    q->rear++;
    q->m_q[q->rear] = ptr;
    return q->rear;
  }
  
  
  
  int i,j;
 
  for (i = 0; i <= q->rear; i++)
  {
      //if (ptr >= q->m_q[i])
      if(q->comparer(q->m_q[i], ptr) > -1)
      {
          while(q->comparer(q->m_q[i], ptr) == 0){
            i = i+1;
          }
          for (j = q->rear + 1; j > i; j--)
          {
              q->m_q[j] = q->m_q[j - 1];
          }
          q->m_q[i] = ptr;
          q->rear++;
          return i;
      }
  }
  q->m_q[i] = ptr;

  q->rear++;
	return i;
}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
  if(q->rear != -1){
    return q->m_q[0];
  }
	return NULL;
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
	return priqueue_remove_at(q, 0);
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
  if(index <= q->rear){
    return q->m_q[index];
  }
	return NULL;
}


/**
  Removes all instances of ptr from the queue. 
  
  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
  int sum = 0;
  if(q->front == -1 && q->rear == -1){
    return 0;
  }
  for(int i = q->front; i < q->rear; i++){
    if(ptr == q->m_q[i]){
      sum++;
      priqueue_remove_at(q, i);
      i--;
    }
  }
	return sum;
}


/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{
  if ((q->front==-1) && (q->rear==-1))
  {
    //printf("\nQueue is empty no elements to delete");
    return NULL;
  }
  if(index > q->rear || index < 0){
    return NULL;
  }
  int i = index;
  void* result = q->m_q[index];
  for(; i < q->rear; i++){
    q->m_q[i] = q->m_q[i+1];
  }
  q->rear--;
  if(q->rear == -1){
    q->front = -1;
  }
  return result;
}


/**
  Return the number of elements in the queue.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
	return q->rear + 1;
}


/**
  Destroys and frees all the memory associated with q.
  
  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
  free(q->m_q);
}

void resize(priqueue_t *q){
  void** new_q = malloc(q->size*sizeof(void*));
  for(int i = 0; i < q->rear; i++){
    new_q[i] = q->m_q[i];
  }
  q->m_q = new_q;
}
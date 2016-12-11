/* fut_queue.c - enqueue, dequeue */

#include <fut_queue.h>

/*queue manipulation functions */

fut_queue *fut_qcreate()
{
  fut_queue *q=(fut_queue *)getmem(sizeof(fut_queue));
  if(q==NULL)
    return NULL;
  q->front=NULL;
  q->rear=NULL;
  return q;
}

fut_qnode* newNode(pid32 pid)
{
  fut_qnode *qnode=(fut_qnode *)getmem(sizeof(fut_qnode));
  if(qnode==NULL)
    return NULL;
  qnode->next=NULL;
  qnode->key=pid;
  return qnode;
}

int fut_enqueue(fut_queue *q, pid32 pid)
{
  fut_qnode *temp=newNode(pid);
  if(temp==NULL)
    return -1;

  if(q->rear==NULL)
  {
    q->front=q->rear=temp;
    return 0;
  }

  q->rear->next=temp;
  q->rear=temp;
  return 0;
}

pid32 fut_dequeue(fut_queue *q)
{
  pid32 pid;
  fut_qnode *temp;
  
  if(q->front==NULL)
    return -1;

  pid=q->front->key;
  temp=q->front;
  q->front=q->front->next;

  if(!freemem(temp,sizeof(fut_qnode)))
    return -1;

  if(q->front==NULL)
    q->rear=NULL;

  return pid; 
}

int fut_isempty(fut_queue *q)
{
  if(q->front==NULL)
    return 1;
  return 0;
}

#include <future.h>

future* future_alloc(int future_flag)
{
  future *f;
  intmask mask;

  mask=disable();

  f=(future *)getmem(sizeof(future));	//allocating memory to new future

  if(f==NULL)
  {
    printf("\nError allocating memory for future variable");
    restore(mask);
    return NULL;
  }

  f->value=(int *)getmem(sizeof(int));	//allocating to member of struct future

  if(f->value==NULL)
  {
    printf("\nError allocating memory for value in future variable");
    restore(mask);
    return NULL;
  }

  f->set_queue=fut_qcreate();
  if(f->set_queue==NULL)
  {
    restore(mask);
    return NULL;
  }

  f->get_queue=fut_qcreate();
  if(f->get_queue==NULL)
  {
    restore(mask);
    return NULL;
  }

  f->flag=future_flag;	//initializing flag for EXCLUSIVE mode
  f->state=FUTURE_EMPTY;	//initializing state of the variable
  f->pid=-1;	//initializing pid
  *(f->value)=0;
  restore(mask);
  return f;
}

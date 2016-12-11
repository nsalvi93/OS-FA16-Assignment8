#include <future.h>

syscall future_set(future *f, int *value)
{
  intmask mask;
  pid32 temp_pid;

  mask=disable();

  if(f->flag==FUTURE_EXCLUSIVE)
  {
    if(f->state==FUTURE_EMPTY || f->state==FUTURE_WAITING)
    {
      f->state=FUTURE_VALID;
      *(f->value)=*value;
    }
    else
    {
      restore(mask);
      return SYSERR;
    }
  }
  else if(f->flag==FUTURE_SHARED)
  {
    if(f->state==FUTURE_VALID)
    {
      restore(mask);
      return SYSERR;
    }

    *(f->value)=*value;
    if(f->state==FUTURE_EMPTY)
      f->state=FUTURE_VALID;

    if(f->state==FUTURE_WAITING)
    {
      f->state=FUTURE_VALID;
      while(!fut_isempty(f->get_queue))
      {
        temp_pid=fut_dequeue(f->get_queue);
        if(temp_pid==-1)
        {
          restore(mask);
          return -1;
        }
        resume(temp_pid);        
      }//while end      
    }//waiting end
  }//shared end

  else if(f->flag==FUTURE_QUEUE)
  {
    if(fut_isempty(f->get_queue))
    {
      f->pid=getpid();
      fut_enqueue(f->set_queue,f->pid);
      suspend(f->pid);
      *(f->value)=*value;
    }
    else
    {
      *(f->value)=*value;
      temp_pid=fut_dequeue(f->get_queue);
      if(temp_pid==-1)
      {
        restore(mask);
        return -1;
      }
      resume(temp_pid); 
    }   
  }

  restore(mask);
  return OK;
}

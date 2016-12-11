#include <future.h>

syscall future_free(future* f)
{
  intmask mask;
  mask=disable();
  if(!freemem(f->value,sizeof(int)))
  {
    restore(mask);
    return -1;
  }

//free all waiting nodes also

  if(!freemem(f->set_queue,sizeof(fut_queue)))
  {
    restore(mask);
    return -1;
  }

  if(!freemem(f->get_queue,sizeof(fut_queue)))
  {
    restore(mask);
    return -1;
  }

  if(!freemem(f,sizeof(future)))
  {
    restore(mask);
    return -1;
  }
  restore(mask);
  return 0;
}

#include <future.h>

int future_prod(future *fut) 
{
  int i,status;
  int j;
  j = (int)fut;

  for (i=0; i<1000; i++) 
  {
    j += i;
  }
  kprintf("\nProducer produced %d",j);
  status=future_set(fut, &j);
  if (status < 1)
  {
    printf("future_set failed\n");
    return -1;
  }
  return OK;
}

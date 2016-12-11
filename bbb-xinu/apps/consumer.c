#include <prodcons.h>

void consumer(int count)
{
      //Use system call wait() and signal() with predefined semaphores 
      //produced and consumed to synchronize critical section
      //Code to consume values of global variable 'n' until the value
      //of n is less than or equal to count
      //print consumed value e.g. consumed : 8
  int i;
  for(i=1;i<=count;i++)
  {
    wait(produced);
    printf("\nConsumed value: %d",n);
    signal(consumed);
  }
  semdelete(produced);
  semdelete(consumed);
}

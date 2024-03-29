#include <xinu.h>

void print_mem()
{
  int i=0;
  struct memblk *curr;
  curr=&memlist;
  kprintf("\nBlock No\tMemory Address\tAvailable memory");
  while(curr!=NULL)
  {
    if(i==0)
      kprintf("\nHead \t\t %u \t %u",curr,curr->mlength);
    else
      kprintf("\n%d \t\t %u \t %u",i,curr,curr->mlength);
    curr=curr->mnext;
    i++;
  }
  kprintf("\n");
}

shellcmd xsh_avail_mem(int nargs, char *args[])
{
  int *p1,*p2,*p3;
  if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) 
  {
    printf("\nHELP");
    printf("\n\tDisplay free memory available");
    printf("\n\tUsage avail_mem");
    printf("\n\n\t--help\tdisplay this help and exit\n");
    return 0;
  }

  if(nargs>1)
  {
    fprintf(stderr,"\n%s: many Arguments...!!!",args[0]);
    fprintf(stderr,"\nUsage avail_mem");
    return 1;
  }
  kprintf("\nInitial memory");
  print_mem(); 

  p1=(int *)getmem(10*sizeof(int));
  if(p1==NULL)
    return 1;

  p2=(int *)getmem(10*sizeof(int));
  if(p2==NULL)
    return 1;

  p3=(int *)getmem(10*sizeof(int));
  if(p3==NULL)
    return 1;

  kprintf("\nAfter allocating memory for 3 pointers of 10 integers each");
  print_mem(); 

  kprintf("\nAfter deallocating memory of second pointer");
  if(!freemem(p2,10*sizeof(int)))
    return 1;
  print_mem(); 

  kprintf("\nAfter deallocating memory of remaining pointers");
  if(!freemem(p1,10*sizeof(int)))
    return 1;
  if(!freemem(p3,10*sizeof(int)))
    return ;
  print_mem(); 

  return 0;
}

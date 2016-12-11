#include <prodcons.h>
#include <future.h>

int n=0;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

sid32 produced,consumed;
future *f1,*f2,*f3,*f_exclusive,*f_shared,*f_queue;
int count;

shellcmd xsh_prodcons(int nargs, char *args[])
{
  //Argument verifications and validations

  int count;             //local varible to hold count
  int flag_sem=1;
  n=0;
  count=0;

  if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) 
  {
    printf("\nHELP");
    printf("\n\tProducer consumer problem");
    printf("\n\tUsage prodcons [number] -- default 2000");
    printf("\n\n\tFuture Implementation");
    printf("\n\tUsage prodcons -f");
    printf("\n\n\t--help\tdisplay this help and exit\n");
    return 0;
  }

  if(nargs>2)
  {
    fprintf(stderr,"\n%s: many Arguments...!!!",args[0]);
    fprintf(stderr,"\nUsage prodcons [number]");
    return 1;
  }
  else if(nargs==2)
  { 
    if(strncmp(args[1],"-f",2)==0 && strlen(args[1])==2)
      flag_sem=0;
    else    //check args[1] if present assign value to count
    {
      count=atoi(args[1]);
      if(count<=0)
      {
        printf("\nPlease enter a valid value.",count);
        printf("\nUsage:");
        printf("\n\tprodcons [number] -- default 2000");
        printf("\n\tprodcons -f\n");
        return 1;
      }
    }
  }
  else 
    count=2000;
  
  if(flag_sem)
  {
    produced = semcreate(0);
    consumed = semcreate(1);

    //create the process producer and consumer and put them in ready queue.
    //Look at the definations of function create and resume in exinu/system folder for reference.      
    resume( create(producer, 1024, 20, "producer", 1, count) );
    resume( create(consumer, 1024, 20, "consumer", 1, count) );
  }
  else
  {/*
    f1 = future_alloc(FUTURE_EXCLUSIVE);
    f2 = future_alloc(FUTURE_EXCLUSIVE);
    f3 = future_alloc(FUTURE_EXCLUSIVE);

    if(f1)
    {
      resume( create(future_cons, 1024, 20, "fcons1", 1, f1) );
      resume( create(future_prod, 1024, 20, "fprod1", 1, f1) );
    }
    else
      printf("\nError creating future f1");

    if(f2)
    {
      resume( create(future_cons, 1024, 20, "fcons2", 1, f2) );
      resume( create(future_prod, 1024, 20, "fprod2", 1, f2) );
    }
    else
      printf("\nError creating future f2");

    if(f3)
    {
      resume( create(future_cons, 1024, 20, "fcons3", 1, f3) );
      resume( create(future_prod, 1024, 20, "fprod3", 1, f3) );
    }
    else
      printf("\nError creating future f3");
*/
    f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
    f_shared = future_alloc(FUTURE_SHARED);
    f_queue = future_alloc(FUTURE_QUEUE);
 
// Test FUTURE_EXCLUSIVE
    if(f_exclusive)
    {
      resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
      resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive) );
    }
    else
      printf("\nError creating future f_exclusive");


// Test FUTURE_SHARED
    if(f_shared)
    {
      resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
      resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
      resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
      resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
      resume( create(future_prod, 1024, 20, "fprod2", 1, f_shared) );
    }
    else
      printf("\nError creating future f_shared");

// Test FUTURE_QUEUE
    if(f_queue)
    {
      resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
      resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
      resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
      resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
      resume( create(future_prod, 1024, 20, "fprod3", 1, f_queue) );
      resume( create(future_prod, 1024, 20, "fprod4", 1, f_queue) );
      resume( create(future_prod, 1024, 20, "fprod5", 1, f_queue) );
      resume( create(future_prod, 1024, 20, "fprod6", 1, f_queue) );
    }
    else
      printf("\nError creating future f_queue");
  }  
  return 0;
}

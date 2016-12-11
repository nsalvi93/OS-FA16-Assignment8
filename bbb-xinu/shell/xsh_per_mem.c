#include <xinu.h>

void proc1(int pid_parent)
{
  int *p;
  kprintf("\nTotal space before process %d start using XINU code %u",getpid(),memlist.mlength);
  p=(int *)getmem(50*sizeof(int));
  if(p==NULL)
    return SYSERR;
  kprintf("\nTotal space after allocating dynamic memory using XINU code %u",memlist.mlength);
  if(!freemem(p,50*sizeof(int)))
    return SYSERR;
  send(pid_parent,"");
  return;
}

void proc2(int pid_parent)
{
  int *p;
  kprintf("\n\nTotal space before process %d starts using OUR code %u",getpid(),memlist.mlength);
  p=(int *)mygetmem(50*sizeof(int));
  if(p==NULL)
    return SYSERR;
  kprintf("\nTotal space after allocating dynamic memory using OUR code %u",memlist.mlength);
  send(pid_parent,"");
  return;
}

shellcmd xsh_per_mem(int nargs, char *args[])
{
  pid32 pid1,pid2;
  if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) 
  {
    printf("\nHELP");
    printf("\n\tProgram for permanent memory allocation");
    printf("\n\tUsage per_mem");
    printf("\n\n\t--help\tdisplay this help and exit\n");
    return 0;
  }

  if(nargs>1)
  {
    fprintf(stderr,"\n%s: many Arguments...!!!",args[0]);
    fprintf(stderr,"\nUsage per_mem");
    return 1;
  }
  pid1=create(proc1,1024,20,"p1",1,getpid());
  resume(pid1);
  receive();
  kprintf("\nTotal space after process termination using XINU code %u",memlist.mlength);

  pid2=create(proc2,1000,20,"p2",2,getpid(),1111);
  resume(pid2);
  receive();
  kprintf("\nTotal space after process termination using OUR code %u",memlist.mlength);

  kprintf("\n");
  return 0;
}

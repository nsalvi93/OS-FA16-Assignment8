#include <xinu.h>

int i=5;

void proc(int pid_parent,int flag, int i)
{
  int a,b,c;
  if(i>0)
  {
    i--;
    proc(pid_parent,flag,i);
  }
  i++;
  if(i==5)
	send(pid_parent,"");
  return;
}

shellcmd xsh_max_stack(int nargs, char *args[])
{
  pid32 pid;

  if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) 
  {
    printf("\nHELP");
    printf("\n\tProgram for maximum stack used");
    printf("\n\tUsage max_stack [recursive iterations]");
    printf("\n\n\t--help\tdisplay this help and exit\n");
    return 0;
  }

  if(nargs>2)
  {
    fprintf(stderr,"\n%s: many Arguments...!!!",args[0]);
    fprintf(stderr,"\nUsage max_stack [recursive iterations]");
    return 1;
  }
  else if(nargs==2)
  { 
    i=atoi(args[1]);
    if(i<=0)
    {
      kprintf("\nPlease enter a valid value.",i);
      kprintf("\nUsage:");
      kprintf("\n\tmax_stack [number of recursive iteration] -- default 5");
      return 1;
    }
  }
  kprintf("\nMemory before process start %u",memlist.mlength);
  pid=create(proc,1024,20,"p1",3,getpid(),1112,i);
  resume(pid);
  receive();
  kprintf("\nMemory after process end %u",memlist.mlength);
 
  kprintf("\n");
  return 0;
}
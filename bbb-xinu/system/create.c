/* create.c - create, newpid */

#include <xinu.h>

local	int newpid();

#define	roundew(x)	( (x+3)& ~0x3)

/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
pid32	create(
	  void		*procaddr,	/* procedure address		*/
	  uint32	ssize,		/* stack size in words		*/
	  pri16		priority,	/* process priority > 0		*/
	  char		*name,		/* name (for debugging)		*/
	  uint32	nargs,		/* number of args that follow	*/
	  ...
	)
{
	uint32		savsp, *pushsp;
	intmask 	mask;    		/* interrupt mask		*/
	pid32		pid;			/* stores new process id	*/
	struct	procent	*prptr;			/* pointer to proc. table entry */
	int32		i;
	uint32		*a,*b;			/* points to list of args	*/
	uint32		*saddr,*temp_saddr;	/* stack address		*/
	uint32		temp;			/* used to store nargs temp	*/
	int16		temp_del_flag;
	int16		max_stack;

	mask = disable();

	max_stack=0;
	temp_del_flag=0;		/*default set to 0 inorder to free stack*/
        b = (uint32 *)(&nargs+1);
        b += nargs -1;
        temp=nargs;
        for(;temp > 0; temp--)
	{
		if(*b==1111)
			temp_del_flag=1;
		if(*b==1112)
			max_stack=1;
		b--;
	}

	if (ssize < MINSTK)
		ssize = MINSTK;
	ssize = (uint32) roundew(ssize);

	if(temp_del_flag==0)
	{
		if (((saddr = (uint32 *)getstk(ssize)) ==
		    (uint32 *)SYSERR ) ||
		    (pid=newpid()) == SYSERR || priority < 1 ) {
			restore(mask);
			return SYSERR;
		}
	}
	else if(temp_del_flag==1)
	{
		if (((saddr = (uint32 *)mygetstk(ssize)) ==
		    (uint32 *)SYSERR ) ||
		    (pid=newpid()) == SYSERR || priority < 1 ) {
			restore(mask);
			return SYSERR;
		}
	}

	prcount++;
	prptr = &proctab[pid];

	/* initialize process table entry for new process */
	prptr->prstate = PR_SUSP;	/* initial state is suspended	*/
	prptr->prprio = priority;
	prptr->prstkbase = (char *)saddr;
	prptr->prstklen = ssize;
	prptr->prname[PNMLEN-1] = NULLCH;
	for (i=0 ; i<PNMLEN-1 && (prptr->prname[i]=name[i])!=NULLCH; i++)
		;
	prptr->prsem = -1;
	prptr->prparent = (pid32)getpid();
	prptr->prhasmsg = FALSE;

	/* set up initial device descriptors for the shell		*/
	prptr->prdesc[0] = CONSOLE;	/* stdin  is CONSOLE device	*/
	prptr->prdesc[1] = CONSOLE;	/* stdout is CONSOLE device	*/
	prptr->prdesc[2] = CONSOLE;	/* stderr is CONSOLE device	*/

	prptr->del_flag=temp_del_flag;
	prptr->stack_flag=max_stack;

	/* Initialize stack as if the process was called		*/

	*saddr = STACKMAGIC;
	temp_saddr = saddr;
	savsp = (uint32)saddr;

	if(max_stack)
		for(i=0; i<prptr->prstklen - sizeof(uint32) ; i++)
			*--temp_saddr = 0XFF;

	/* push arguments */
	a = (uint32 *)(&nargs + 1);	/* start of args		*/
	a += nargs -1;			/* last argument		*/
	for ( ; nargs > 4 ; nargs--)	/* machine dependent; copy args	*/
		*--saddr = *a--;	/* onto created process' stack	*/
	*--saddr = (long)procaddr;
	for(i = 11; i >= 4; i--)
		*--saddr = 0;
	for(i = 4; i > 0; i--) {
		if(i <= nargs)
			*--saddr = *a--;
		else
			*--saddr = 0;
	}
	*--saddr = (long)INITRET;	/* push on return address	*/
	*--saddr = (long)0x00000053;	/* CPSR, A, F bits set,		*/
					/* Supervisor mode		*/
	prptr->prstkptr = (char *)saddr;
	restore(mask);
	return pid;

	/* The following entries on the stack must match what ctxsw	*/
	/*   expects a saved process state to contain: ret address,	*/
	/*   ebp, interrupt mask, flags, registerss, and an old SP	*/

	*--saddr = (long)procaddr;	/* Make the stack look like it's*/
					/*  half-way through a call to	*/
					/*  ctxsw that "returns" to the	*/
					/*  new process			*/
	*--saddr = savsp;		/* This will be register ebp	*/
					/*  for process exit		*/
	savsp = (uint32) saddr;		/* start of frame for ctxsw	*/
	*--saddr = 0x00000200;		/* New process runs with	*/
					/*  interrupts enabled		*/

	/* Basically, the following emulates a x86 "pushal" instruction	*/

	*--saddr = 0;		/* %eax */
	*--saddr = 0;		/* %ecx */
	*--saddr = 0;		/* %edx */
	*--saddr = 0;		/* %ebx */
	*--saddr = 0;		/* %esp; value filled in below */
	pushsp = saddr;		/*  remember this location */
	*--saddr = savsp;	/* %ebp (while finishing ctxsw) */
	*--saddr = 0;		/* %esi */
	*--saddr = 0;		/* %edi */
	*pushsp = (unsigned long) (prptr->prstkptr = (char *)saddr);
	restore(mask);
	return pid;
}

/*------------------------------------------------------------------------
 *  newpid  -  Obtain a new (free) process ID
 *------------------------------------------------------------------------
 */
local	pid32	newpid(void)
{
	uint32	i;			/* iterate through all processes*/
	static	pid32 nextpid = 1;	/* position in table to try or	*/
					/*  one beyond end of table	*/

	/* check all NPROC slots */

	for (i = 0; i < NPROC; i++) {
		nextpid %= NPROC;	/* wrap around to beginning */
		if (proctab[nextpid].prstate == PR_FREE) {
			return nextpid++;
		} else {
			nextpid++;
		}
	}
	return (pid32) SYSERR;
}

/* mygetstk.c - mygetstk */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  mygetstk  -  Allocate stack memory, returning highest word address
 *------------------------------------------------------------------------
 */
char  	*mygetstk(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr;	/* Walk through memory list	*/
	struct	memblk	*fits, *fitsprev; /* Record block that fits	*/

	mask = disable();
	kprintf("\nCalling new stack function: mygetstsk");
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	prev = &memlist;
	curr = memlist.mnext;
	fits = NULL;
	fitsprev = NULL;  /* Just to avoid a compiler warning */

	while (curr != NULL) {			/* Scan entire list	*/
		if (curr->mlength >= nbytes) {	/* Record block address	*/
			fits = curr;		/*   when request fits	*/
			fitsprev = prev;
		}
		prev = curr;
		curr = curr->mnext;
	}

	if (fits == NULL) {			/* No block was found	*/
		restore(mask);
		return (char *)SYSERR;
	}
	if (nbytes == fits->mlength) {		/* Block is exact match	*/
		fitsprev->mnext = fits->mnext;
	} else {				/* Remove top section	*/
		fits->mlength -= nbytes;
		fits = (struct memblk *)((uint32)fits + fits->mlength);
	}
	memlist.mlength -= nbytes;
	restore(mask);
	return (char *)((uint32) fits + nbytes - sizeof(uint32));
}
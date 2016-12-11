/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - say hello to user
 *------------------------------------------------------------------------
 */

shellcmd xsh_hello(int nargs, char *args[]) {

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tDisplays the hello message\n");
		printf("\tOnly single string argument permitted\n");
		printf("Options (one per invocation):\n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}
	
	if(nargs>2){
		fprintf(stderr,"%s: too many arguments\n",args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;	
	}
	
	if(nargs<2){
		fprintf(stderr,"%s: too few arguments\n",args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;	
	}
	
	if(nargs==2){
		printf("Hello %s, Welcome to the world of Xinu!!\n",args[1]);
		return 1;	
	}

	return 0;
}

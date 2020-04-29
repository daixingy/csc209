#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Write the main() function of a program that takes exactly two arguments,
  and prints one of the following:
    - "Same\n" if the arguments are the same.
    - "Different\n" if the arguments are different.
    - "Invalid\n" if the program is called with an incorrect number of
      arguments.

  Your main function should return 0, regardless of what's printed.
*/


	int main(int argc, char **argv) {
	    if(argc != 3) {
	        fprintf(stderr, "Invalid\n");
	        exit(1);
	    }
	    
	    char *name1 = argv[1];
	    char *name2 = argv[2];

	    if (strcmp(name1, name2) == 0){
	    	printf("same\n");
	    }else{
	    	printf("different\n");
	    }
	    

	    return 0;
	}
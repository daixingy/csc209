#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 256

// Error messages 
#define USAGE "Usage: decode textfile codefile\n"
#define INVALID_LINE "Invalid line number\n"
#define EMPTY "Document is empty\n"
#define INVALID_POSITION "Invalid character position for this line\n"
#define LENGTH "Invalid chunk size"


int main(int argc, char *argv[]) {
    // If the user did not enter the correct number of arguments then exit
    if(argc != 3) { 
        fprintf(stderr, USAGE);
        exit(1); 
        // The argument of 1 is a convention indicating the program failed
    }

    FILE *tf; // File pointer for the text document
    FILE *cf; // File pointer for the code file

    /* Open both files for reading. Terminate if there is any problem 
     * opening the files */
    if((tf = fopen(argv[1], "r")) == NULL) {
        perror("fopen");
        exit(1);
    }

    if((cf = fopen(argv[2], "r")) == NULL) {
        perror("fopen");
        exit(1);
    }

    // We will assume that lines read from the text file and the code file
    // will never exceed MAXLINE-1 characters.
    char code_line[MAXLINE]; 
    char text_line[MAXLINE];
    

    
    // Add your implmeentation below.
    char *linecf;
    char *linetf;
    //check if the code file is empty
    if ( (linecf = fgets(code_line,MAXLINE,cf)) == NULL) {
        fprintf(stderr, EMPTY);
        exit(1);
    }

    long previous = -1;
    //keep scanning the code file line by line until all the lines are scanned
    while( linecf != NULL){
        //convert each code line from char to long
        long i;
        long j;
        long k;

        code_line[MAXLINE-1] = '\0';
        char *s = code_line;
        char *leftover;            
        i = strtol(s,&leftover,10);
        j = strtol(leftover,&leftover,10);
        k = strtol(leftover,&leftover,10);

        
        int r = 1;
        //keep scanning until we find the ith line 
        while(r <= (i-previous) ){
            if ( (linetf = fgets(text_line,MAXLINE,tf)) == NULL) {
                fprintf(stderr, EMPTY);
                exit(1);
            }else{
                text_line[MAXLINE-1] = '\0';
                r++;

            }

        }

        previous = i;

        //print out the char at position j and the lenth is k
        if(!text_line[j]){
             fprintf(stderr, INVALID_POSITION);
             exit(1); 
        } else if((j+k) > strlen(text_line)){
             fprintf(stderr, LENGTH);
             exit(1);
         }else{
             char dest[MAXLINE];
             int c = 0;
             while(c < k){
                dest[c] = text_line[j+c];
                c++;
             }
             dest[c] = '\n';
             printf("%s\n",dest );
        }

        linecf = fgets(code_line,MAXLINE,cf);
    }

   
    
    

    // Print a newline after the end of the message so the prompt appears on 
    // the next line.
    printf("\n");
    return 0;
}

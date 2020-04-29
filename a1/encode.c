#include <stdio.h>
#include <stdlib.h>

#include <string.h>  // Only used for strlen


#define MAXLINE 256

// Error messages
#define USAGE "Usage: encode chunksize textfile msg\n"
#define ENCODE_FAILED "Could not encode message.\n"


int main(int argc, char *argv[]) {
    // Check to make sure the user entered the correct number of arguments
    if(argc != 4) { 
        fprintf(stderr, USAGE);
        exit(1); 
        // The argument of 1 is a convention indicating the program failed
    }

    FILE *tf; // File pointer for the text document

    // determine message chunks size
    int chunk_size = strtol(argv[1], NULL, 0);  
    if(chunk_size == 0) {
        fprintf(stderr, "Invalid chunk size\n");
        exit(1);
    }
    
    if((tf = fopen(argv[2], "r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    
    // The message to encode is in argv[3].  We will use a pointer to char
    // to give it a better name.
    char *message = argv[3];
    int msg_length = strlen(message);  // only allowed use of string functions
    
    // Complete the implmentation below
    // seperate the message by white space into an array of chars
    char mesg[MAXLINE];
    strncpy(mesg,message,msg_length);
    char new_string[MAXLINE][MAXLINE];
    int i,j,ctr;
    i = 0; ctr = 0;
    for(i=0;i<= msg_length;i++){
        if(mesg[i]==' ' || mesg[i]=='\0'){
            new_string[ctr][j]='\0';
            ctr++;
            j=0;
        }else{
            new_string[ctr][j]=mesg[i];
            j++;
        }
    }

    
    int n = 0;
    char text_line[MAXLINE] = {0x0};
    int count = 0;
    char *ret;
    //kepp scanning the file line by line until all the lines have been scanned
    while(fgets(text_line, MAXLINE, tf) != NULL){

        //printing out the code of the current chunk when the current char is found 
        if ((n <= (strlen(*new_string))) && ((ret = strstr(text_line, new_string[n])) != NULL)){
            printf("%d\t%ld\t%lu\n", count,strlen(text_line)-strlen(ret),strlen(new_string[n]));
            n++;
        }

        //add the number of the line by 1 each time after this line has beem checked
        count++;

    }

    //if after the execution of the code above, n is till smaller than length of new_string(the total 
    //number of char that needs to be encoded) which means that there are some chars left and can not 
    //be found in the text file. Thus the encoding process failed.
    if( n < strlen(*new_string) ){
        fprintf(stderr, ENCODE_FAILED);
        exit(1); 
    }

    fclose(tf);
    



    
    

    return 0;
}

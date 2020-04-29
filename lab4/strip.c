#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Complete this program by writing the function strip_q_marks that takes
    a single string and returns an integer.

    The function should modify the string to remove any trailing question marks
    and return the number of question marks that were removed.

    Examples
    original sentence       modified sentence       return value
    =============================================================
    "Hello? World???"       "Hello? World"          3
    "What..?"               "What.."                1
    "Apples?..?"            "Apples?.."             1
    "Coffee"                "Coffee"                0
*/

// Write the function strip_q_marks here
    int strip_q_marks(char *c){
        int count = 0;
        int i;
        char *q = "?";
        for(i=(strlen(c)-1);i>=0;i--){

            char *ch = &c[i];
            ch[1] = '\0';
            if(strcmp(ch,q) == 0){
                count += 1;
            }else{
                return count;
            }
        }

        return count;
    }


    int main(int argc, char **argv) {
        if(argc != 2) {
            fprintf(stderr, "Usage: strip message\n");
            exit(1);
        }
        int result = strip_q_marks(argv[1]);
        printf("%s %d\n", argv[1], result);
        return 0;
    }

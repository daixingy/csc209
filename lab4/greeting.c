#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GREETING

/*
    This program has two arguments: the first is a greeting message, and the
    second is a name.

    The message is an impersonal greeting, such as "Hi" or "Good morning".
    name is set to refer to a string holding a friend's name, such as
    "Emmanuel" or "Xiao".

    First copy the first argument to the array greeting. (Make sure it is
    properly null-terminated.)

    Write code to personalize the greeting string by appending a space and
    then the string pointed to by name.
    So, in the first example, greeting should be set to "Hi Emmanuel", and
    in the second it should be "Good morning Xiao".

    If there is not enough space in greeting, the resulting greeting should be
    truncated, but still needs to hold a proper string with a null terminator.
    For example, "Good morning" and "Emmanuel" should result in greeting
    having the value "Good morning Emmanu".
*/

    int main(int argc, char **argv) {
        if (argc != 3) {
            fprintf(stderr, "Usage: greeting message name\n");
            exit(1);
        }
        char greeting[20];
        char *name = argv[2];

        if(strlen(argv[1])<20){
            strncpy(greeting,argv[1],strlen(argv[1]));
            greeting[strlen(argv[1])] = '\0';
        }else{
            strncpy(greeting,argv[1],19);
            greeting[19] = '\0';
        }

        printf("%s\t%s\n", greeting,name);
        return 0;
    }



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "bitmap.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define ERROR_MESSAGE "Warning: one or more filter had an error, so the output image may not be correct.\n"
#define SUCCESS_MESSAGE "Image transformed successfully!\n"


/*
 * Check whether the given command is a valid image filter, and if so,
 * run the process.
 *
 * We've given you this function to illustrate the expected command-line
 * arguments for image_filter. No further error-checking is required for
 * the child processes.
 */
void run_command(const char *cmd) {
    if (strcmp(cmd, "copy") == 0 || strcmp(cmd, "./copy") == 0 ||
        strcmp(cmd, "greyscale") == 0 || strcmp(cmd, "./greyscale") == 0 ||
        strcmp(cmd, "gaussian_blur") == 0 || strcmp(cmd, "./gaussian_blur") == 0 ||
        strcmp(cmd, "edge_detection") == 0 || strcmp(cmd, "./edge_detection") == 0) {
        execl(cmd, cmd, NULL);
    } else if (strncmp(cmd, "scale", 5) == 0) {
        // Note: the numeric argument starts at cmd[6]
        execl("scale", "scale", cmd + 6, NULL);
    } else if (strncmp(cmd, "./scale", 7) == 0) {
        // Note: the numeric argument starts at cmd[8]
        execl("./scale", "./scale", cmd + 8, NULL);
    } else {
        fprintf(stderr, "Invalid command '%s'\n", cmd);
        exit(1);
    }
}


// TODO: Complete this function.
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: image_filter input output [filter ...]\n");
        exit(1);
    }

    

    if(argc == 3){
        int fd[2];
        if(pipe(fd) == -1){
            perror("pipe");
            exit(1);
        }
        int result = fork();
        if(result <0){
            perror("fork");
        }else if(result == 0){
            int image = open(argv[1],O_RDONLY);
            dup2(image,fileno(stdin));
            int n_image = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR|S_IWUSR);
            dup2(n_image, fileno(stdout));
            char *copy = "./copy";
            run_command(copy);
            if(close(fd[0]) == -1){
                perror("close");
            }
            if(close(fd[1]) == -1){
                perror("close");
            }    

        }

    }else{
        int quantity = argc - 3;
        int fd[quantity][2];
        for(int i=0;i<quantity;i++){

            if(pipe(fd[i])== -1){
                perror("pipe");
                exit(1);
            }

            int result = fork();

            if(result < 0){
                perror("fork");

            }else if (result == 0) {
                if(i==0){
                    int image = open(argv[1],O_RDONLY);
                    if(dup2(image,fileno(stdin))== -1){
                        perror("dup2");
                        exit(1);
                    }
                   
                } else{
                    if(dup2(fd[i-1][0],fileno(stdin))==-1){
                        perror("dup2");
                        exit(1);
                    }

                }

                if(i == quantity-1){
        
                    int n_image = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR|S_IWUSR);
                    if(dup2(n_image, fileno(stdout))==-1){
                        perror("dup2");
                        exit(1);
                    }

                }else{
                    if(dup2(fd[i][1],fileno(stdout))==-1){
                        perror("dup2");
                        exit(1);
                    }

                }
                
                for(int j = 0; j <= i; j++) {
                    if(close(fd[j][0]) == -1) {
                        perror("close");
                    }
                    if(close(fd[j][1]) == -1){
                        perror("close");
                    }
                }    

                run_command(argv[i+3]);
            }

        }

        for(int j = 0; j <quantity; j++) {
                if(close(fd[j][0]) == -1){
                    perror("close");
                }
                if(close(fd[j][1]) == -1){
                    perror("close");
                }    
        }           

        int status;
        for(int i = 0;i<quantity;i++){
            if(wait(&status) == -1){
                printf(ERROR_MESSAGE);
            }
        }
    }    


    printf(SUCCESS_MESSAGE);
    return 0;
}

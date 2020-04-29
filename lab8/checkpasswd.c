#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256
#define MAX_PASSWORD 10

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {

  char user_id[MAXLINE];
  char password[MAXLINE];
  int fd[2];

  if(fgets(user_id, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  if(fgets(password, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }

  if(pipe(fd) == -1){
    perror("pipe");
  }

  int r = fork();
  if(r >0){
     if ((close(fd[0])) == -1) {
            perror("close");
      }

     if(write(fd[1],user_id,MAX_PASSWORD) == -1){
        perror("write to pipe");
      }
     if(write(fd[1],password,MAX_PASSWORD) == -1){
        perror("write to pipe");
      }
      close(fd[1]);
      int status;
      int c = 0;
     if(wait(&status) != -1){
        if(WIFEXITED(status)){
          c = WEXITSTATUS(status); 
        }else{
          printf("%s\n","value return error");
        }
      }
      
      if(c == 0){
        printf("%s", SUCCESS );
      }else if(c == 2){
        printf("%s", INVALID );
      }else if(c == 3){
        printf("%s", NO_USER );
      }else{
        printf("%s", "error");
      }


  }else if(r == 0){
    if(close(fd[1]) == -1){
      perror("close");
    }
    if(dup2(fd[0],fileno(stdin)) == -1 ){
      perror("dup2");
      exit(1);
    }
     execl("./validate", "./validate", (char *) 0);

  }else{
    perror("fork");
    exit(1);
  }
  


  return 0;
}

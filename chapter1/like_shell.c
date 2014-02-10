#include <sys/types.h>
#include <sys/wait.h>
#include "../lib/ourhdr.h"

int main (void) {
  char  buf[MAXLINE];
  pid_t pid;
  int   status;

  printf("%% ");
  while (fgets(buf, MAXLINE, stdin) != NULL) {
    buf[strlen(buf) - 1] = 0;

    if((pid = fork()) < 0) {
      printf("error");
      exit(0);
    } else if(pid == 0) {
      execlp(buf, buf, (char *) NULL);
      printf("error");
      exit(0);
    }

    if ((pid == waitpid(pid, &status, 0)) < 0) {
      exit(0);
    }
    printf("%% ");
  }
  exit(0);
}

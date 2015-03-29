#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void pr_exit(int status) {
  if (WIFEXITED(status)) {
    printf("nomal termination, exit status = %d\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status), WCOREDUMP(status) ? " (core file generated" : "");
  } else if (WIFSTOPPED(status)) {
    printf("child stopped, signal number = %d\n", WSTOPSIG(status));
  }
}


int main () {
  pid_t pid;
  int status = 0;

  if ((pid = fork()) == 0) {
    printf("child\n");
  } else {
    //wait(&status);
    //pr_exit(status);
    sleep(10);
  }
  return 0;
}

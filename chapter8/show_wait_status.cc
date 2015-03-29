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
  int status;

  // 1st test
  if ((pid = fork()) < 0) {
    fprintf(stderr, "fork error\n");
    exit(100);
  } else if (pid == 0) {
    //child
    exit(7);
  }

  if (wait(&status) != pid) {
    fprintf(stderr, "wait error\n");
    exit(100);
  } else {
    pr_exit(status);
  }

  // 2nd test
  if ((pid = fork()) < 0) {
    fprintf(stderr, "fork error\n");
    exit(100);
  } else if (pid == 0) {
    //child
    abort();
  }

  if (wait(&status) != pid) {
    fprintf(stderr, "wait error\n");
    exit(100);
  } else {
    pr_exit(status);
  }

  // 3rd
  if ((pid = fork()) < 0) {
    fprintf(stderr, "fork error\n");
    exit(100);
  } else if (pid == 0) {
    //child
    status /= 0;
  }

  if (wait(&status) != pid) {
    fprintf(stderr, "wait error\n");
    exit(100);
  } else {
    pr_exit(status);
  }

  return 0;
}

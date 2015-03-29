#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
  pid_t pid;
  
  if ((pid = fork()) < 0) {
    fprintf(stderr, "fork error\n");
    exit(10);
  } else if (pid == 0) {
    // first child
    if ((pid = fork()) < 0) {
      fprintf(stderr, "fork error\n");
      exit(10);
    } else if (pid > 0) {
      // first child suiide  immediately
      exit(0);
    }

    // second child
    sleep(2);
    printf("second child, parent pid = %ld\n", (long)getppid());
    exit(0);
  }

  // all parent
  if (waitpid(pid, NULL, 0) != pid) {
    fprintf(stderr, "waitpid error");
  }
  printf("exit parent\n");

  return 0;
}

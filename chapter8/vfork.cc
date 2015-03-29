#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int globvar = 6;

int main(void)
{
  int var;
  pid_t pid;

  var = 88;
  printf("before vfork\n");
  if ((pid = vfork()) < 0) {
    perror("vfork error\n");
    exit(1);
  } else if (pid == 0) {
    globvar++;
    var++;
    printf("vfork child: pid = %ld, globvar = %d, var = %d\n", (long)getpid(), globvar, var);
    _exit(0);
  }

  printf("vfork : pid = %ld, globvar = %d, var = %d\n", (long)getpid(), globvar, var);
  exit(0);
}

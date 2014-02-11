#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../lib/ourhdr.h"

/* result
$ od -c file.hole
0000000    a   b   c   d   e   f   g   h   i   j  \0  \0  \0  \0  \0  \0
0000020   \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
0000040   \0  \0  \0  \0  \0  \0  \0  \0   A   B   C   D   E   F   G   H
0000060    I   J
0000062
*/

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void) {
  int fd;

  if((fd = open("file.hole", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) < 0) {
    // create error
    exit(1);
  }

  if (write(fd, buf1, 10) != 10) {
    // write error
    exit(1);
  }

  if (lseek(fd, 40, SEEK_SET) == -1) {
    // seek error
    exit(1);
  }

  if (write(fd, buf2, 10) != 10) {
    // write error
    exit(1);
  }

  return 0;
}

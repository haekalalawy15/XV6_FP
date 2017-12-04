#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  char buf[512];
  int fd, dfd, r=0,w=0;
  char *src;
  char *dest;

  src = argv[1];
  dest = argv[2];
  fd = open(src, O_RDONLY);
  dfd = open(dest, O_CREATE|O_WRONLY);

  close(fd);
  close(dfd);

  exit();
}

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(void)
{
  char buf[64];
  struct stat st;

  int fd = open("mydata.txt", O_CREATE | O_WRONLY);
  if (fd < 0) {
    printf("open for write failed\n");
    exit(1);
  }
  printf("write fd = %d\n", fd);

  int n = write(fd, "hello xv6\n", 10);
  printf("wrote %d bytes\n", n);
  close(fd);

  fd = open("mydata.txt", O_RDONLY);
  printf("read  fd = %d\n", fd);

  n = read(fd, buf, sizeof(buf) - 1);
  if (n < 0) {
    printf("read failed\n");
    exit(1);
  }
  buf[n] = 0;
  printf("read %d bytes: %s", n, buf);

  fstat(fd, &st);
  printf("size = %ld bytes, inode = %d\n", st.size, st.ino);

  close(fd);
  exit(0);
}
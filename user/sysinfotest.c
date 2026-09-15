#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

int
main(void)
{
  struct sysinfo info;

  if (sysinfo(&info) < 0) {
    printf("sysinfo failed\n");
    exit(1);
  }
  printf("freepages = %lu pages\n", info.freepages);
  printf("nproc     = %lu\n", info.nproc);

  // TODO ③  fork a child, call sysinfo again, and see nproc change

  int pid = fork();

  if (pid < 0) {
    printf("fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    for (;;)
      ;
  }

  if (sysinfo(&info) < 0) {
    printf("sysinfo failed\n");
    exit(1);
  }

  printf("after fork:\n");
  printf("freepages = %lu pages\n", info.freepages);
  printf("nproc     = %lu\n", info.nproc);

  kill(pid);

  wait(0);

  exit(0);
}
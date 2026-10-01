#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int global = 42;

void
show(char *name, void *va)
{
  printf("%s  va %p -> pa %p\n",
         name, va, (void *)va2pa((uint64)va));
}

int
main(void)
{
  int local = 7;

  show("text ", (void *)main);
  show("data ", &global);
  show("stack", &local);

  exit(0);
}
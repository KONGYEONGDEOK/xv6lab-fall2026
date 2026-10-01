#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
uint64
sys_va2pa(void)
{
  uint64 va;
  pte_t *pte;
  struct proc *p = myproc();

  argaddr(0, &va);

  if (va >= MAXVA)
    return 0;

  // TODO ①
// va에서 L2, L1, L0 페이지 테이블 인덱스와
// 페이지 내부 오프셋을 추출해서 출력한다.
// PX(level, va)는 각 단계의 인덱스를 구하고,
// va & 0xFFF는 하위 12비트 오프셋을 구한다.

  printk("va %p : L2=%lu L1=%lu L0=%lu off=0x%lx\n",
       (void *)va,
       PX(2, va),
       PX(1, va),
       PX(0, va),
       va & 0xFFF);

  pte = walk(p->pagetable, va, 0);    // alloc = 0 : 찾기만 하고 만들지 않는다
  if (pte == 0 || (*pte & PTE_V) == 0)
    return 0;                         // 매핑이 없다
  if ((*pte & PTE_U) == 0)
    return 0;                         // 매핑은 있지만 사용자용이 아니다

// TODO ②
// PTE2PA(*pte)로 물리 페이지의 시작 주소를 구하고,
// va의 하위 12비트 오프셋을 더해서 최종 물리 주소를 반환한다.

  return PTE2PA(*pte) + (va & 0xFFF);
}
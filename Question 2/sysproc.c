#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Assignment 10 (Q1b): System call to display physical memory statistics
int
sys_memstats(void)
{
  return memstats_info();
}

// Assignment 10 (Q2a): System call int vtop(int virtual_address)
int
sys_vtop(void)
{
  int va;
  if(argint(0, &va) < 0)
    return -1;
  return vtop_translate(myproc()->pgdir, (uint)va);
}

// Assignment 10 (Q3a): System call int pageinfo(int virtual_address)
int
sys_pageinfo(void)
{
  int va;
  if(argint(0, &va) < 0)
    return -1;
  return pageinfo_inspect(myproc()->pgdir, (uint)va);
}

// Assignment 10 (Q3b): System call int setreadonly(int virtual_address)
int
sys_setreadonly(void)
{
  int va;
  if(argint(0, &va) < 0)
    return -1;
  return page_setreadonly(myproc()->pgdir, (uint)va);
}

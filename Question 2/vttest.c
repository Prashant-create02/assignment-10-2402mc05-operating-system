#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(int argc, char *argv[])
{
  char *base;
  int va1, va2, va3, invalid_va;
  int pa1, pa2, pa3, invalid_pa;

  printf(1, "=== Question 2: Virtual-to-Physical Address Translation Test ===\n\n");

  // Allocate 2 pages using sbrk()
  base = sbrk(2 * PGSIZE);
  if(base == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }

  // Test 1: Code segment address (main), Page 1 base, Page 2 with offset 0x123
  va1 = (int)main;
  va2 = (int)base;
  va3 = (int)(base + PGSIZE + 0x123);

  pa1 = vtop(va1);
  pa2 = vtop(va2);
  pa3 = vtop(va3);

  printf(1, "1. Code Address (main)        : VA = 0x%x -> PA = 0x%x\n", va1, pa1);
  printf(1, "2. Heap Page 1 (offset 0x000) : VA = 0x%x -> PA = 0x%x\n", va2, pa2);
  printf(1, "3. Heap Page 2 (offset 0x123) : VA = 0x%x -> PA = 0x%x\n", va3, pa3);

  // Test 2: Unmapped/Invalid virtual address well beyond process heap
  invalid_va = 0x40000000;
  invalid_pa = vtop(invalid_va);
  printf(1, "4. Unmapped Virtual Address   : VA = 0x%x -> Return = %d (%s)\n",
         invalid_va, invalid_pa, (invalid_pa == -1) ? "INVALID / UNMAPPED" : "MAPPED");

  exit();
}

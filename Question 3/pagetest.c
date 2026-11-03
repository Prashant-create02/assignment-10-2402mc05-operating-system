#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(int argc, char *argv[])
{
  char *page;

  printf(1, "=== Question 3: Page Table Management & Protection Test ===\n\n");

  // 1. Allocate one page aligned on a page boundary using sbrk()
  page = sbrk(PGSIZE);
  if(page == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }

  // Write while the page still has PTE_W enabled
  page[0] = 'A';
  printf(1, "[Step 1] Allocated page at VA = 0x%x. Initial write succeeded (page[0] = '%c').\n",
         (uint)page, page[0]);

  // 2. Display initial page-table information (expect P=1, W=1, U=1)
  printf(1, "[Step 2] Inspecting PTE before setreadonly():\n");
  pageinfo((int)page);

  // 3. Remove write permission (PTE_W) using setreadonly()
  printf(1, "\n[Step 3] Calling setreadonly(0x%x)...\n", (uint)page);
  if(setreadonly((int)page) < 0){
    printf(1, "setreadonly failed!\n");
    exit();
  }

  // 4. Verify updated page-table flags (expect P=1, W=0, U=1)
  printf(1, "[Step 4] Inspecting PTE after setreadonly():\n");
  pageinfo((int)page);

  // 5. Attempt to write to the read-only page -> triggers Trap 14 (T_PGFLT)
  printf(1, "\n[Step 5] Attempting to write to read-only page at VA = 0x%x...\n", (uint)page);
  printf(1, "(Expecting kernel trap 14 [Page Fault] to terminate this process)\n");
  page[0] = 'B';

  // Execution will never reach this line due to the page fault
  printf(1, "ERROR: Write succeeded unexpectedly!\n");
  exit();
}

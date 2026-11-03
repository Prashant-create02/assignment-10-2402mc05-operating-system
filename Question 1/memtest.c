#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(int argc, char *argv[])
{
  int num_pages = 5;
  int bytes = num_pages * PGSIZE;
  char *ptr;

  printf(1, "=== Question 1: Memory Allocation Tracking Test ===\n\n");

  printf(1, "[1] Initial Memory Statistics:\n");
  memstats();

  printf(1, "\n[2] Allocating %d pages (%d bytes) using sbrk()...\n", num_pages, bytes);
  ptr = sbrk(bytes);
  if(ptr == (char*)-1){
    printf(1, "Error: sbrk allocation failed!\n");
    exit();
  }
  memstats();

  printf(1, "\n[3] Releasing %d pages (-%d bytes) using sbrk()...\n", num_pages, bytes);
  if(sbrk(-bytes) == (char*)-1){
    printf(1, "Error: sbrk deallocation failed!\n");
    exit();
  }
  memstats();

  exit();
}

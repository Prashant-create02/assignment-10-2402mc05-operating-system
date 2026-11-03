#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_FRAMES 10
#define REF_LEN    20

struct stats {
  int faults;
  int hits;
};

// Helper to print current frame contents
void
print_frames(int frames[], int num_frames)
{
  int i;
  printf(1, "[ ");
  for(i = 0; i < num_frames; i++){
    if(frames[i] == -1)
      printf(1, "- ");
    else
      printf(1, "%d ", frames[i]);
  }
  printf(1, "]");
}

// Helper to print a 2-decimal-place ratio (num / den) and percentage without %f
void
print_ratio(int num, int den)
{
  int ratio_cent = (num * 100) / den; // e.g., 5/20 -> 25 -> 0.25
  printf(1, "0.");
  if(ratio_cent < 10)
    printf(1, "0%d", ratio_cent);
  else
    printf(1, "%d", ratio_cent);
  printf(1, " (%d%%)", ratio_cent);
}

// 1. FIFO Page Replacement Algorithm
struct stats
run_fifo(int refs[], int n, int num_frames, int verbose)
{
  int frames[MAX_FRAMES];
  int fifo_ptr = 0;
  int i, j, hit;
  struct stats s = {0, 0};

  for(i = 0; i < num_frames; i++)
    frames[i] = -1;

  if(verbose)
    printf(1, "\n--- FIFO Page Replacement (%d Frames) ---\n", num_frames);

  for(i = 0; i < n; i++){
    int page = refs[i];
    hit = 0;

    for(j = 0; j < num_frames; j++){
      if(frames[j] == page){
        hit = 1;
        break;
      }
    }

    if(hit){
      s.hits++;
    } else {
      s.faults++;
      frames[fifo_ptr] = page;
      fifo_ptr = (fifo_ptr + 1) % num_frames;
    }

    if(verbose){
      printf(1, "Ref %d: ", page);
      print_frames(frames, num_frames);
      printf(1, " -> %s\n", hit ? "HIT" : "PAGE FAULT");
    }
  }

  if(verbose)
    printf(1, "Total Page Faults (FIFO, %d frames): %d | Total Hits: %d\n",
           num_frames, s.faults, s.hits);

  return s;
}

// 2. LRU (Least Recently Used) Page Replacement Algorithm
struct stats
run_lru(int refs[], int n, int num_frames, int verbose)
{
  int frames[MAX_FRAMES];
  int last_used[MAX_FRAMES];
  int i, j, hit;
  struct stats s = {0, 0};

  for(i = 0; i < num_frames; i++){
    frames[i] = -1;
    last_used[i] = -1;
  }

  if(verbose)
    printf(1, "\n--- LRU Page Replacement (%d Frames) ---\n", num_frames);

  for(i = 0; i < n; i++){
    int page = refs[i];
    hit = 0;

    for(j = 0; j < num_frames; j++){
      if(frames[j] == page){
        hit = 1;
        last_used[j] = i;
        break;
      }
    }

    if(hit){
      s.hits++;
    } else {
      int replace_idx = -1;
      s.faults++;

      // Check for an empty frame first
      for(j = 0; j < num_frames; j++){
        if(frames[j] == -1){
          replace_idx = j;
          break;
        }
      }

      // Otherwise find the frame with minimum last_used timestamp
      if(replace_idx == -1){
        int min_time = 999999;
        for(j = 0; j < num_frames; j++){
          if(last_used[j] < min_time){
            min_time = last_used[j];
            replace_idx = j;
          }
        }
      }

      frames[replace_idx] = page;
      last_used[replace_idx] = i;
    }

    if(verbose){
      printf(1, "Ref %d: ", page);
      print_frames(frames, num_frames);
      printf(1, " -> %s\n", hit ? "HIT" : "PAGE FAULT");
    }
  }

  if(verbose)
    printf(1, "Total Page Faults (LRU, %d frames): %d | Total Hits: %d\n",
           num_frames, s.faults, s.hits);

  return s;
}

// 3. Optimal Page Replacement Algorithm
struct stats
run_optimal(int refs[], int n, int num_frames, int verbose)
{
  int frames[MAX_FRAMES];
  int i, j, k, hit;
  struct stats s = {0, 0};

  for(i = 0; i < num_frames; i++)
    frames[i] = -1;

  if(verbose)
    printf(1, "\n--- Optimal Page Replacement (%d Frames) ---\n", num_frames);

  for(i = 0; i < n; i++){
    int page = refs[i];
    hit = 0;

    for(j = 0; j < num_frames; j++){
      if(frames[j] == page){
        hit = 1;
        break;
      }
    }

    if(hit){
      s.hits++;
    } else {
      int replace_idx = -1;
      s.faults++;

      // Check for an empty frame first
      for(j = 0; j < num_frames; j++){
        if(frames[j] == -1){
          replace_idx = j;
          break;
        }
      }

      // Otherwise replace the page whose next use is furthest in the future
      if(replace_idx == -1){
        int farthest = -1;
        for(j = 0; j < num_frames; j++){
          int next_use = 999999;
          for(k = i + 1; k < n; k++){
            if(frames[j] == refs[k]){
              next_use = k;
              break;
            }
          }
          if(next_use > farthest){
            farthest = next_use;
            replace_idx = j;
          }
        }
      }

      frames[replace_idx] = page;
    }

    if(verbose){
      printf(1, "Ref %d: ", page);
      print_frames(frames, num_frames);
      printf(1, " -> %s\n", hit ? "HIT" : "PAGE FAULT");
    }
  }

  if(verbose)
    printf(1, "Total Page Faults (Optimal, %d frames): %d | Total Hits: %d\n",
           num_frames, s.faults, s.hits);

  return s;
}

void
print_row(char *alg, int f, struct stats s, int n)
{
  printf(1, "%s\t\t%d\t%d\t%d\t", alg, f, s.faults, s.hits);
  print_ratio(s.hits, n);
  printf(1, "\t");
  print_ratio(s.faults, n);
  printf(1, "\n");
}

int
main(int argc, char *argv[])
{
  int refs[REF_LEN] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
  int frame_counts[3] = {3, 4, 5};
  int idx;

  printf(1, "=== Question 4: Page Replacement Algorithms (FIFO, LRU, Optimal) ===\n");
  printf(1, "Reference String (20 refs): 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1\n");

  // Task 4a & 4b: Detailed step-by-step trace for 3 frames
  run_fifo(refs, REF_LEN, 3, 1);
  run_lru(refs, REF_LEN, 3, 1);
  run_optimal(refs, REF_LEN, 3, 1);

  // Task 4c: Summary Comparison Table for 3, 4, and 5 frames
  printf(1, "\n========================================================================\n");
  printf(1, "Algorithm\tFrames\tFaults\tHits\tHit Ratio\tFault Ratio\n");
  printf(1, "========================================================================\n");

  for(idx = 0; idx < 3; idx++){
    int f = frame_counts[idx];
    struct stats sf = run_fifo(refs, REF_LEN, f, 0);
    struct stats sl = run_lru(refs, REF_LEN, f, 0);
    struct stats so = run_optimal(refs, REF_LEN, f, 0);

    print_row("FIFO   ", f, sf, REF_LEN);
    print_row("LRU    ", f, sl, REF_LEN);
    print_row("Optimal", f, so, REF_LEN);
    printf(1, "------------------------------------------------------------------------\n");
  }

  exit();
}

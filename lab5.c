#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  struct header *curr = free_list_ptr;
  while (curr != NULL) {
    if (curr->size >= size) {
      return curr->id;
    }
    curr = curr->next;
  }
  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  uint64_t best_size = 0;
  struct header *curr = free_list_ptr;
  while (curr != NULL) {
    if (curr->size >= size) {
      if (best_fit_id == -1 || curr->size < best_size) {
        best_fit_id = curr->id;
        best_size = curr->size;
      }
    }
    curr = curr->next;
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  uint64_t worst_size = 0;
  struct header *curr = free_list_ptr;
  while (curr != NULL) {
    if (curr->size >= size) {
      if (worst_fit_id == -1 || curr->size > worst_size) {
        worst_fit_id = curr->id;
        worst_size = curr->size;
      }
    }
    curr = curr->next;
  }
  return worst_fit_id;
}

int main(void) {
  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

/*
 * part 2
 *
 * Assumptions:
 * idea: when a lock z is freed, check whether a free block sits directly before it or directly
 * after it in memory. If so, merge them into one bigger free block. This assumes the existing free
 * list is already coalesced, so only z needs to be checked.
 *
 * two block are contiguous if the first one's memory ends exactly where the second one starts.  A
 * block's end = start + header sze + its size
 *
 * algorithm:
 *
 * 1. go through the free list once. for each free block:
 *    - if it ends exactly where z starts, remember as "before"
 *    -if it starts exactly where z ends, remember as "after", and also remember the block that
comes
 *   just before it in the list so it can be removed later
 *
 * 2. depending on what was found:
 *
 *    Case A, neither found:
 *      z has no free neighbors, just add z to free list
 *
 *    Case B, only "before" found:
 *      grow "before" to also cover z: add z's size plus one header to before's size. "before" is
 *      already in the list so its done
 *
 *    Case C, only "after: found:
 *      grow z to also cover "after": add after's size pllus one header to z's size. the put z in
 *      the list in place of "after"
 *
 *    Case D, both found
 *      gorw "before" tot cover z and "after": add both sizes plus two headers to before's size.
 *      Remove "after" from the list since it is now part of "before"
 *
 *Each merge adds one headers's worth of space, becasue the header of the block being absorbed
 *becomes ordinary usable memory
 *
 *the list is a scanned once, so this take O(n) time
 *
 *Example
 * free list is head -> n -> b -> m, and z (between m and n) is freed.
 *the scan finds before =m and after = n, so this case is D
 * m grows to cover z and n, and n is removed from the list
 *result: head -> b -> m, where m now covers the old m + z + n
*/

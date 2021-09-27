#include <stdlib.h>
#include <allocator.h>

svm_free_f svm_free = &free;
svm_malloc_f svm_malloc = &malloc;
svm_calloc_f svm_calloc = &calloc;
svm_realloc_f svm_realloc = &realloc;

void svm_set_allocators(struct allocators allocs) {
  svm_free = allocs.free;
  svm_malloc = allocs.malloc;
  svm_calloc = allocs.calloc;
  svm_realloc = allocs.realloc;
}

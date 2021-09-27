#pragma once
#include <stddef.h>

typedef void (*svm_free_f)(void* old);
typedef void* (*svm_malloc_f)(size_t size);
typedef void* (*svm_calloc_f)(size_t count, size_t size);
typedef void* (*svm_realloc_f)(void* old, size_t new_size);

struct allocators {
  svm_free_f free;
  svm_malloc_f malloc;
  svm_calloc_f calloc;
  svm_realloc_f realloc;
};

void svm_set_allocators(struct allocators allocs);

extern svm_free_f svm_free;
extern svm_malloc_f svm_malloc;
extern svm_calloc_f svm_calloc;
extern svm_realloc_f svm_realloc;

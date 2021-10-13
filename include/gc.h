#pragma once
#include <common.h>
#include <objects/object.h>

#define GC_MARKED BIT(0)
#define GC_COLLECTABLE BIT(1)

typedef struct {
  size_t marked;
  size_t deleted;
} gc_stat_t;


gc_stat_t gc_round(svm_object_t **objs);

size_t gc_mark(svm_object_t *obj);
size_t gc_sweep();

gc_stat_t gc_get_global_stat();

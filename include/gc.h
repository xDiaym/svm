#pragma once
#include <common.h>
#include <stdlib.h>

#define GC_MARKED BIT(0)
#define GC_COLLECTABLE BIT(1)

#define IS_MARKED(object) (AS_SVM_OBJECT(object)->gc_flags & GC_MARKED)
#define IS_COLLECTABLE(object) \
  (AS_SVM_OBJECT(object)->gc_flags & GC_COLLECTABLE)

typedef struct {
  size_t round;
  size_t alive;
  size_t marked;
  size_t deleted;
} gc_stat_t;


typedef struct _svm_object_type svm_object_type;
typedef struct _svm_object svm_object_t;


svm_object_t *svm_object_create(svm_object_type *type, size_t object_size);
#define CREATE_OBJECT(object)                                                  \
  (object *)svm_object_create(&TYPE_NAME(object), sizeof(object))

svm_object_t *retain(svm_object_t *obj);
#define RETAIN(x) retain(AS_SVM_OBJECT(x))

void release(svm_object_t *obj);
#define RELEASE(x) release(AS_SVM_OBJECT(x))

gc_stat_t gc_round(svm_object_t **objs);

gc_stat_t gc_get_global_stat();
void gc_print_stat(gc_stat_t stat);

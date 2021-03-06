#pragma once
#include <objects/object.h>

typedef struct {
  SVM_OBJECT_HEAD;
} null_object_t;

/*
 * There is only 1 null object.
 * Kind on singleton: On first call it will create object, on other calls it
 * will return created instance
 */
null_object_t *get_null_object();
#define SVM_NULL get_null_object()

extern svm_object_type TYPE_NAME(null_object_t);

#pragma once
#include <objects/object.h>

typedef struct _null_object {
  SVM_OBJECT_HEAD;
} null_object;

/*
 * There is only 1 null object.
 * Kind on singleton: On first call it will create object, on other calls it
 * will return created instance
 */
null_object *get_null_object();
#define SVM_NULL get_null_object()

extern svm_object_type null_object_type;

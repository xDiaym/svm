#include <assert.h>
#include <object.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct _int_object {
  SVM_OBJECT_HEAD;
  int64_t value;
} int_object;

svm_object *int_object_from_int(int64_t value);
int64_t int_object_to_int(svm_object *int_);

extern svm_object_type int_object_type;

#include <assert.h>
#include <objects/object.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct _int_object {
  SVM_OBJECT_HEAD;
  int64_t value;
} int_object;

int_object *int_object_from_int(int64_t value);

extern svm_object_type int_object_type;

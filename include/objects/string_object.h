#include <assert.h>
#include <objects/object.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct _string_object {
  SVM_OBJECT_HEAD;
  size_t length;
  char *data;
} string_object;

string_object *string_object_from_c_str(const char *c_str);
string_object *string_object_from_str(const char *c_str, size_t length);

extern svm_object_type string_object_type;

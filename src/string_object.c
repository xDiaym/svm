#include <int_object.h>
#include <panic.h>
#include <stdlib.h>
#include <string_object.h>

svm_object *string_object_from_c_str(const char *c_str) {
  size_t length = strlen(c_str);
  string_object *str = CREATE_OBJECT(string_object, &string_object_type);
  str->length = length;
  str->data = (char *)malloc(length * sizeof(char));
  assert(str->data); // TODO:
  strcpy(str->data, c_str);
  return AS_SVM_OBJECT(str);
}

static svm_object *string_object_from_str(const char *c_str, size_t length) {
  string_object *str = CREATE_OBJECT(string_object, &string_object_type);
  str->length = length + 1;
  str->data = calloc(str->length, sizeof(char));
  assert(str->data); // TODO:
  memcpy(str->data, c_str, length);
  str->data[length] = '\0';
  return AS_SVM_OBJECT(str);
}

static void string_desctructor(svm_object *this) {
  free(((string_object *)this)->data);
}

static svm_object *string_object_concat(svm_object *this, svm_object *other) {
  if (!SAME_TYPE(this, other)) {
    panic("Attemp to concat string object with non-string object.");
  }
  // TODO: add length method
  string_object *this_s = AS_TYPE(string_object, this),
                *other_s = AS_TYPE(string_object, other);
  size_t length = this_s->length + other_s->length;
  string_object *str = CREATE_OBJECT(string_object, &string_object_type);
  str->length = length;
  str->data = (char *)malloc(length * sizeof(char));
  assert(str->data); // TODO:
  memcpy(str->data, this_s->data, this_s->length);
  memcpy(str->data + this_s->length, other_s->data, str->length);
  return (svm_object *)str;
}

static svm_object *string_object_index(svm_object *this, svm_object *index) {
  if (!HAS_TYPE(int_object_type, index)) {
    panic("Attemp to index string object with non-int object.");
  }
  string_object *string = AS_TYPE(string_object, this);
  int64_t idx = int_object_to_int(index);
  if (idx >= string->length) {
    panic("Index out of range.");
  }
  return string_object_from_str(string->data + idx, 1);
}

static svm_object *string_to_string(svm_object *this) { return this; }

svm_object_type string_object_type = {
    /*destructor_method=*/&string_desctructor,
    /*call_method=*/NULL,
    /*index_method=*/&string_object_index,
    /*add_method=*/&string_object_concat,
    /*to_string_method=*/&string_to_string,
};

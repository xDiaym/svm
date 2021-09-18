#include <panic.h>
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

static svm_object *string_to_string(svm_object *this) { return this; }

svm_object_type string_object_type = {
    /*destructor_method=*/&string_desctructor,
    /*call_method=*/NULL,
    /*index_method=*/NULL, // TODO
    /*add_method=*/&string_object_concat,
    /*to_string_method=*/&string_to_string,
};

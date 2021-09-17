#include <string_object.h>

svm_object *string_object_from_c_str(const char *c_str) {
  size_t length = strlen(c_str);
  string_object *str = CREATE_OBJECT(string_object, &string_object_type);
  str->length = length;
  str->data = (char *)malloc(length * sizeof(char));
  assert(str->data); // TODO:
  strcpy(str->data, c_str);
  return (svm_object *)str;
}

static void string_desctructor(svm_object *this) {
  free(((string_object *)this)->data);
}

static svm_object *string_to_string(svm_object *this) { return this; }

static svm_object_type string_object_type = {
    /*desctructor_method=*/string_desctructor,
    /*call_method=*/NULL,
    /*index_method=*/NULL, // TODO
    /*add_method=*/NULL,   // TODO
    /*to_string_method=*/string_to_string,
};

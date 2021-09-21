#include <objects/int_object.h>
#include <objects/string_object.h>
#include <panic.h>
#include <stdlib.h>

string_object *string_object_from_c_str(const char *c_str) {
  string_object *str = CREATE_OBJECT(string_object, &string_object_type);
  str->length = strlen(c_str);
  str->data = calloc(str->length, sizeof(char));
  assert(str->data); // TODO:

  strcpy(str->data, c_str);

  return str;
}

string_object *string_object_from_str(const char *c_str, size_t length) {
  string_object *str = CREATE_OBJECT(string_object, &string_object_type);
  str->length = length + 1;
  str->data = calloc(str->length, sizeof(char));
  assert(str->data); // TODO:

  memcpy(str->data, c_str, length);
  str->data[length] = '\0';

  return str;
}

static void string_destructor(string_object *this) {
  free(this->data);
}

static string_object *string_object_concat(string_object *this, svm_object *other) {
  if (!HAS_TYPE(string_object_type, other)) {
    panic("Attempt to concat string object with non-string object.");
  }
  // TODO: add length method
  string_object *other_s = CAST_TO(string_object, other);

  string_object *str = CREATE_OBJECT(string_object, &string_object_type);
  str->length = this->length + other_s->length;
  str->data = calloc(str->length, sizeof(char));
  assert(str->data); // TODO:

  memcpy(str->data, this->data, this->length);
  memcpy(str->data + this->length, other_s->data, str->length);

  return str;
}

static string_object *string_object_index(string_object *this, svm_object *index) {
  if (!HAS_TYPE(int_object_type, index)) {
    panic("Attempt to index string object with non-int object.");
  }
  int64_t idx = CAST_TO(int_object, index)->value;
  if ((size_t)idx >= this->length) {
    panic("Index out of range.");
  }
  return string_object_from_str(this->data + idx, 1);
}

static string_object *string_to_string(string_object *this) { return this; }

svm_object_type string_object_type = {
    /*destructor_method=*/(desctructor_method)&string_destructor,
    /*call_method=*/NULL,
    /*index_method=*/(index_method)&string_object_index,
    /*add_method=*/(add_method)&string_object_concat,
    /*to_string_method=*/(to_string_method)&string_to_string,
};

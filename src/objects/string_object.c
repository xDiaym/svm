#include <allocator.h>
#include <objects/int_object.h>
#include <objects/string_object.h>
#include <panic.h>
#include <stdlib.h>

string_object_t *string_object_from_c_str(const char *c_str) {
  string_object_t *str = CREATE_OBJECT(string_object_t, &string_object_type);
  str->length = strlen(c_str);
  str->data = svm_calloc(str->length, sizeof(char));
  assert(str->data); // TODO:

  strcpy(str->data, c_str);

  return str;
}

string_object_t *string_object_from_str(const char *c_str, size_t length) {
  string_object_t *str = CREATE_OBJECT(string_object_t, &string_object_type);
  str->length = length + 1;
  str->data = svm_calloc(str->length, sizeof(char));
  assert(str->data); // TODO:

  memcpy(str->data, c_str, length);
  str->data[length] = '\0';

  return str;
}

static void string_destructor(string_object_t *this) { free(this->data); }

static string_object_t *string_object_concat(string_object_t *this,
                                           svm_object_t *other) {
  if (!HAS_TYPE(string_object_type, other)) {
    panic("Attempt to concat string object with non-string object.");
  }
  // TODO: add length method
  string_object_t *other_s = CAST_TO(string_object_t, other);

  string_object_t *str = CREATE_OBJECT(string_object_t, &string_object_type);
  str->length = this->length + other_s->length;
  str->data = svm_calloc(str->length, sizeof(char));
  assert(str->data); // TODO:

  memcpy(str->data, this->data, this->length);
  memcpy(str->data + this->length, other_s->data, str->length);

  return str;
}

static string_object_t *string_object_index(string_object_t *this,
                                          svm_object_t *index) {
  if (!HAS_TYPE(int_object_type, index)) {
    panic("Attempt to index string object with non-int object.");
  }
  int64_t idx = CAST_TO(int_object_t, index)->value;
  if ((size_t)idx >= this->length) {
    panic("Index out of range.");
  }
  return string_object_from_str(this->data + idx, 1);
}

static string_object_t *string_to_string(string_object_t *this) { return this; }

svm_object_type string_object_type = {
    .m_destructor = (desctructor_method)&string_destructor,
    .m_index = (index_method)&string_object_index,
    .m_add = (add_method)&string_object_concat,
    .m_to_string = (to_string_method)&string_to_string,
};

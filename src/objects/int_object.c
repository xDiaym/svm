#include <allocator.h>
#include <objects/int_object.h>
#include <objects/object.h>
#include <objects/string_object.h>
#include <panic.h>
#include <stdio.h>

int_object_t *int_object_from_int(int64_t value) {
  int_object_t *int_ = CREATE_OBJECT(int_object_t);
  int_->value = value;
  return int_;
}

static int_object_t *int_object_add(int_object_t *this, svm_object_t *other) {
  if (!SAME_TYPE(this, other)) {
    panic("Attempt to concat string object with non-string object.");
  }
  int_object_t *s = CAST_TO(int_object_t, other);
  return int_object_from_int(this->value + s->value);
}

static size_t int_dec_length(int64_t value) {
  return snprintf(NULL, 0, "%li", value);
}

static string_object_t *int_object_to_string(int_object_t *this) {
  size_t length = int_dec_length(this->value);
  char *buff = calloc(length, sizeof(char));
  sprintf(buff, "%li", this->value);
  string_object_t *str = string_object_from_str(buff, length);
  svm_free(buff);
  return str;
}

svm_object_type TYPE_NAME(int_object_t) = {
    .m_add = (add_method)&int_object_add,
    .m_to_string = (to_string_method)&int_object_to_string,
};

#include <int_object.h>
#include <panic.h>
#include <stdio.h>
#include <string_object.h>

svm_object *int_object_from_int(int64_t value) {
  int_object *int_ = CREATE_OBJECT(int_object, &int_object_type);
  int_->value = value;
  return AS_SVM_OBJECT(int_);
}

static void int_desctructor(svm_object *this) {}

static svm_object *int_object_add(svm_object *this, svm_object *other) {
  if (!SAME_TYPE(this, other)) {
    panic("Attemp to concat string object with non-string object.");
  }
  int_object *f = AS_TYPE(int_object, this), *s = AS_TYPE(int_object, other);
  int64_t sum = f->value + s->value;
  int_object *int_ = CREATE_OBJECT(int_object, int_object_from_int(sum));
  return AS_SVM_OBJECT(int_);
}

static size_t int_dec_length(int64_t value) {
  size_t len = 1;
  if (value < 0)
    ++len;
  while (value > 10) {
    ++len;
    value %= 10;
  }
  return len;
}

static svm_object *int_object_to_string(svm_object *this) {
  int_object *int_ = AS_TYPE(int_object, this);
  char *buff = calloc(int_dec_length(int_->value), sizeof(char));
  sprintf(buff, "%li", int_->value);
  string_object *str = string_object_from_c_str(buff);
  free(buff);
  return AS_SVM_OBJECT(str);
}

svm_object_type int_object_type = {
    /*destructor_method=*/&int_desctructor,
    /*call_method=*/NULL,
    /*index_method=*/NULL,
    /*add_method=*/&int_object_add,
    /*to_string_method=*/&int_object_to_string,
};

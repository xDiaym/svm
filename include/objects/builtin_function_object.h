#pragma once
#include <objects/object.h>

typedef svm_object_t *(*c_api_function)(svm_object_t **args);

typedef struct {
  SVM_OBJECT_HEAD;
  const char *name;
  c_api_function function;
} builtin_function_object_t;

builtin_function_object_t *builtin_function_from_c(const char *name,
                                                 c_api_function func);

extern svm_object_type TYPE_NAME(builtin_function_object_t);

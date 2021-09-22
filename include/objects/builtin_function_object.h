#pragma once
#include <objects/object.h>

typedef svm_object *(*c_api_function)(svm_object **args);

typedef struct _builtin_function_object {
  SVM_OBJECT_HEAD;
  const char *name;
  c_api_function function;
} builtin_function_object;

builtin_function_object *builtin_function_from_c(const char *name,
                                                 c_api_function func);

extern svm_object_type builtin_function_object_type;

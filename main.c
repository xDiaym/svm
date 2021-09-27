#include <common.h>
#include <objects/builtin_function_object.h>
#include <objects/null_object.h>
#include <objects/object.h>
#include <objects/string_object.h>
#include <stdio.h>

// prints first arg to stdio
svm_object *print(svm_object **args) {
  string_object *str = RETAIN(string_object, svm_object_to_string(args[0]));
  printf("%s", str->data);
  RELEASE(str);
  return NULL;
}
#include <malloc.h>

int main() {
  builtin_function_object *print_ =
      RETAIN(builtin_function_object, builtin_function_from_c("print", &print));
  svm_object_print_debug_info(AS_SVM_OBJECT(print_));
  svm_object_print_debug_info(AS_SVM_OBJECT(SVM_NULL));

  svm_object *pseudostack[] = {AS_SVM_OBJECT(SVM_NULL)};

  svm_object_call(AS_SVM_OBJECT(print_), pseudostack);

  RELEASE(print_);
  return 0;
}

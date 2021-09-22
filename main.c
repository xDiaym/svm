#include <objects/int_object.h>
#include <objects/object.h>
#include <objects/string_object.h>
#include <objects/builtin_function_object.h>
#include <stdio.h>


// prints first arg to stdio
svm_object *print(svm_object** args) {
  string_object *str = RETAIN(string_object, svm_object_to_string(args[0]));
  printf("%s", str->data);
  RELEASE(str);
  return NULL;
}

int main() {
  string_object *str =
      RETAIN(string_object, string_object_from_c_str("Hello world!"));
  builtin_function_object *print_ = builtin_function_from_c("name", &print);
  svm_object_print_debug_info(AS_SVM_OBJECT(print_));

  svm_object *pseudostack[] = {AS_SVM_OBJECT(str)};

  svm_object_call(print_, &pseudostack);

  RELEASE(str);
  return 0;
}

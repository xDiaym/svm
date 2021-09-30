#include <common.h>
#include <gc.h>
#include <objects/builtin_function_object.h>
#include <objects/int_object.h>
#include <objects/list_object.h>
#include <objects/null_object.h>
#include <objects/object.h>
#include <objects/string_object.h>
#include <stdio.h>

int main() {
  list_object *list = list_object_new();
  svm_object *int_ = RETAIN(int_object_from_int(42));
  list_object_push_back(list, AS_SVM_OBJECT(list));

  gc_mark();

  return 0;
}

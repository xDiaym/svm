#include <allocator.h>
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
  list_object_push_back(list, AS_SVM_OBJECT(list));
  list_object_push_back(list, AS_SVM_OBJECT(list));

  gc_mark();
  gc_sweep();

  return 0;
}

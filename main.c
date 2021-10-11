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
  int_object_t *int_1 = CAST_TO(int_object_t, RETAIN(int_object_from_int(42)));

  list_object_t *list = list_object_new();
  list_object_push_back(list, AS_SVM_OBJECT(list));
  list_object_push_back(list, AS_SVM_OBJECT(list));

  int_object_t *int_2 = CAST_TO(int_object_t, RETAIN(int_object_from_int(42)));

  gc_mark(AS_SVM_OBJECT(int_1));
  gc_mark(AS_SVM_OBJECT(int_2));
  size_t deleted = gc_sweep();

  gc_stat_t stat = gc_get_global_stat();
  printf("Marked: %zu, Deleted: %zu\n", stat.marked, stat.deleted);
  printf("Deleted in 1st round: %zu\n", deleted);

  return 0;
}

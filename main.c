#include <allocator.h>
#include <common.h>
#include <objects/../gc.h> // FIXME: rename gc.h(some file has same name)
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
  list_object_push_back(list, AS_SVM_OBJECT(int_1));

  int_object_t *int_2 = CAST_TO(int_object_t, RETAIN(int_object_from_int(42)));
  safe_cast(int_2, TYPE_NAME(list_object_t));

  svm_object_t *reachable[] = {AS_SVM_OBJECT(list), NULL};
  gc_stat_t round_stat = gc_round(reachable);

  gc_stat_t global_stat = gc_get_global_stat();
  printf("[Global] Marked: %zu, Deleted: %zu\n", global_stat.marked, global_stat.deleted);
  printf("[Round 1] Marked: %zu, Deleted: %zu\n", round_stat.marked, round_stat.deleted);

  return 0;
}

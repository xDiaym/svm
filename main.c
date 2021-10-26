#include <allocator.h>
#include <common.h>
#include <hash_table.h>
#include <objects/builtin_function_object.h>
#include <objects/int_object.h>
#include <objects/list_object.h>
#include <objects/null_object.h>
#include <objects/object.h>
#include <objects/string_object.h>
#include <stdio.h>
#include <svm_gc.h>

int main() {
  int_object_t *int1 = int_object_from_int(42);
  hash_table_t *ht = hash_table_new();

  const char indexes[] = "012345678";
  const size_t is = sizeof(indexes) / sizeof(indexes[0]);

  for (size_t i = 0; i < is; ++i) {
    hash_table_insert_item(ht, &indexes[i], int1);
    svm_object_t *item1 = hash_table_search_item(ht, &indexes[i]);
    printf("[%p] %zu\n", item1, CAST_TO(int_object_t, item1)->value);
  }

  hash_table_delete(ht);

  return 0;
}

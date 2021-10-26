#include <allocator.h>
#include <common.h>
#include <objects/builtin_function_object.h>
#include <objects/int_object.h>
#include <objects/list_object.h>
#include <objects/null_object.h>
#include <objects/object.h>
#include <objects/string_object.h>
#include <stdio.h>
#include <svm_gc.h>
#include <hash_table.h>

int main() {
  int_object_t *int1 = int_object_from_int(42);
  hash_table_t *ht = hash_table_new();

  hash_table_insert_item(ht, "123", int1);
  svm_object_t *item1 = hash_table_search_item(ht, "123");
  printf("[%p] %zu\n", item1, CAST_TO(int_object_t, item1)->value);

  hash_table_delete_item(ht, "123");
  svm_object_t *item2 = hash_table_search_item(ht, "123");
  printf("%p\n", item2);

  hash_table_delete(ht);

  return 0;
}

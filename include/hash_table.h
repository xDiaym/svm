#pragma once
#include <stdlib.h>

typedef struct _svm_object svm_object_t;

typedef struct {
  const char *key;
  svm_object_t *value;
} hash_table_item_t;

typedef struct {
  size_t size;
  size_t capacity;
  hash_table_item_t **items;
} hash_table_t;

hash_table_t *hash_table_new();
void hash_table_delete(hash_table_t *ht);

void hash_table_insert_item(hash_table_t *ht, const char *key,
                            svm_object_t *object);
svm_object_t *hash_table_search_item(hash_table_t *ht, const char *key);
void hash_table_delete_item(hash_table_t *ht, const char *key);

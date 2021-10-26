#include <allocator.h>
#include <hash_table.h>
#include <objects/object.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_INITIAL_SIZE (7)

#define HASH_CONST_1 (37)
#define HASH_CONST_2 (53)


static size_t hash_func(const char *string, const size_t a, const size_t m) {
  size_t hash = 0;
  for (size_t i = 0; i < strlen(string); ++i) {
    hash = (hash + a * string[i]) % m;
  }
  return hash;
}

static size_t hash(const char* string, const size_t m, const size_t attempt) {
  const size_t hash1 = hash_func(string, HASH_CONST_1, m);
  const size_t hash2 = hash_func(string, HASH_CONST_2, m);
  return (hash1 + attempt * hash2) % m;
}

static hash_table_item_t *hash_table_item_new(const char *key,
                                              svm_object_t *value) {
  hash_table_item_t *item = svm_malloc(sizeof(hash_table_item_t));
  item->key = svm_malloc(strlen(key));
  strcpy(item->key, key);
  item->value = RETAIN(value);
  return item;
}

static void hash_table_item_delete(hash_table_item_t *hti) {
  RELEASE(hti->value);
  svm_free(hti->key);
  svm_free(hti);
}

hash_table_t *hash_table_new() {
  hash_table_t *ht = svm_malloc(sizeof(hash_table_t));
  ht->size = 0;
  ht->capacity = HASH_TABLE_INITIAL_SIZE;
  ht->items = svm_calloc(ht->capacity, sizeof(svm_object_t*));

  return ht;
}

void hash_table_delete(hash_table_t *ht) {
  for (size_t i = 0; i < ht->capacity; ++i) {
    if (ht->items[i] != NULL) {
      hash_table_item_delete(ht->items[i]);
    }
  }
  svm_free(ht->items);
  svm_free(ht);
}

void hash_table_insert_item(hash_table_t *ht, const char *key,
                            svm_object_t *object) {
  hash_table_item_t *hti = hash_table_item_new(key, object);
  size_t attempt = 0;
  size_t index = hash(key, ht->capacity, attempt);
  while (ht->items[index] != NULL) {
    ++attempt;
    index = hash(key, ht->capacity, attempt);
  }
  ht->items[index] = hti;
  ++ht->size;
}

svm_object_t *hash_table_search_item(hash_table_t *ht, const char *key) {
  size_t attempt = 0;
  size_t index = hash(key, ht->capacity, attempt);
  hash_table_item_t *item = ht->items[index];
  while (item != NULL) {
    if (strcmp(item->key, key) == 0) {
      return item->value;
    }
    ++attempt;
    index = hash(key, ht->capacity, attempt);
    item = ht->items[index];
  }
  return NULL;
}

void hash_table_delete_item(hash_table_t *ht, const char *key) {
  size_t attempt = 0;
  size_t index = hash(key, ht->capacity, attempt);
  hash_table_item_t *item = ht->items[index];
  while (item != NULL) {
    if (strcmp(item->key, key) == 0) {
      hash_table_item_delete(item);
      ht->items[index] = NULL;
      --ht->size;
      break;
    }
    ++attempt;
    index = hash(key, ht->capacity, attempt);
    item = ht->items[index];
  }
}

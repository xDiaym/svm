#include <allocator.h>
#include <hash_table.h>
#include <math.h>
#include <objects/object.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_INITIAL_SIZE (7)
#define HASH_TABLE_RESIZE_TOP_THRESHOLD (70)
#define HASH_TABLE_RESIZE_DOWN_THRESHOLD (20)

#define HASH_CONST_1 (37)
#define HASH_CONST_2 (53)

static int is_prime(const size_t x) {
  // TODO: use x / 2 instead od sqrt
  size_t top = (size_t)sqrt((double)x) + 1;
  for (size_t i = 2; i <= top; ++i) {
    if (x % i == 0) {
      return 0;
    }
  }
  return 1;
}

static size_t next_prime(size_t start) {
  while (!is_prime(start)) {
    ++start;
  }
  return start;
}

static size_t hash_func(const char *string, const size_t a, const size_t m) {
  size_t hash = 0;
  for (size_t i = 0; i < strlen(string); ++i) {
    hash += (hash + a * string[i]) % m;
  }
  return hash;
}

static size_t hash(const char *string, const size_t m, const size_t attempt) {
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

/**
 * Hash table implementation.
 */

static void hash_table_swap(hash_table_t *left, hash_table_t *right) {
  hash_table_t tmp = *left;
  *left = *right;
  *right = tmp;
}

static hash_table_t *hash_table_new_with_capacity(const size_t capacity) {
  hash_table_t *ht = svm_malloc(sizeof(hash_table_t));
  ht->size = 0;
  ht->capacity = next_prime(capacity);
  ht->items = svm_calloc(ht->capacity, sizeof(svm_object_t *));

  return ht;
}

hash_table_t *hash_table_new() {
  return hash_table_new_with_capacity(HASH_TABLE_INITIAL_SIZE);
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

static void hash_table_resize(hash_table_t *ht, const size_t capacity) {
  if (capacity < HASH_TABLE_INITIAL_SIZE) {
    return;
  }

  hash_table_t *new_ht = hash_table_new_with_capacity(capacity);
  for (size_t i = 0; i < ht->capacity; ++i) {
    const hash_table_item_t *item = ht->items[i];
    if (item != NULL) {
      hash_table_insert_item(new_ht, item->key, item->value);
    }
  }

  hash_table_swap(ht, new_ht);
  hash_table_delete(new_ht);
}

static void hash_table_resize_up(hash_table_t *ht) {
  hash_table_resize(ht, ht->capacity * 2);
}

static void hash_table_resize_down(hash_table_t *ht) {
  hash_table_resize(ht, ht->capacity / 2);
}

void hash_table_insert_item(hash_table_t *ht, const char *key,
                            svm_object_t *object) {
  const size_t fill = ht->size * 100 / ht->capacity;
  if (fill >= HASH_TABLE_RESIZE_TOP_THRESHOLD) {
    hash_table_resize_up(ht);
  }

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
  const size_t fill = ht->size * 100 / ht->capacity;
  if (fill <= HASH_TABLE_RESIZE_DOWN_THRESHOLD) {
    hash_table_resize_down(ht);
  }

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

#pragma once
#include <objects/object.h>

typedef struct _list_node list_node_t;

typedef struct {
  SVM_OBJECT_HEAD;
  list_node_t *head;
  list_node_t *tail;
  size_t size;
} list_object_t;

list_object_t *list_object_new();
void list_object_push_back(list_object_t *this, svm_object_t *object);

extern svm_object_type list_object_type;

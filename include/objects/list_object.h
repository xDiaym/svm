#pragma once
#include <objects/object.h>

typedef struct _list_node list_node_t;

typedef struct _list_object {
  SVM_OBJECT_HEAD;
  list_node_t *head;
  list_node_t *tail;
  size_t size;
} list_object;

list_object *list_object_new();
void list_object_push_back(list_object *this, svm_object *object);

extern svm_object_type list_object_type;

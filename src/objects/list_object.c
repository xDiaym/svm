#include <allocator.h>
#include <objects/int_object.h>
#include <objects/list_object.h>
#include <panic.h>

typedef struct _list_node {
  SVM_OBJECT_HEAD;
  list_node_t *next;
  list_node_t *prev;
  svm_object *value;
} list_node_t;

static list_node_t *list_node_new(svm_object *value) {
  list_node_t *node = svm_malloc(sizeof(list_node_t));
  node->next = node->prev = NULL;
  node->value = value;
  return node;
}

static void list_node_destructor(list_node_t* node) {
    RELEASE(node->value);
}

svm_object_type list_node_type = {
    .m_destructor = (desctructor_method)&list_node_destructor
};

/* list_object section */

list_object *list_object_new() {
  list_object *list = CREATE_OBJECT(list_object, &list_object_type);
  list->tail = list->head = NULL;
  list->size = 0;
  return list;
}

void list_object_push_back(list_object *this, svm_object *object) {
  list_node_t *node = list_node_new(object);
  if (this->head == NULL) {
    this->head = this->tail = node;
  } else {
    this->tail->next = node;
    node->prev = this->tail;
    this->tail = node;
  }
  ++this->size;
}

static void list_destructor(list_object *this) {
  list_node_t *node = this->head;
  while (node != NULL) {
    list_node_t *next = node->next;
    RELEASE(node);
    node = next;
  }
}

static svm_object *list_index(list_object* this, svm_object* index) {
  if (!HAS_TYPE(int_object_type, index)) {
    panic("Attempt to index list object with non-int index");
  }
  uint64_t i = CAST_TO(int_object, index)->value;
  size_t j;
  list_node_t *ptr;
  if (i > this->size / 2) {
    j = this->size;
    ptr = this->tail;
    while (i < j) {
      ptr = ptr->prev;
      --j;
    }
  } else {
    j = 0;
    ptr = this->head;
    while (j < i) {
      ptr = ptr->next;
      ++j;
    }
  }

  return ptr->value;
}

svm_object_type list_object_type = {
    .m_destructor = (desctructor_method)&list_destructor,
    .m_index = (index_method)&list_index,
};

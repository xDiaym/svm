#include <allocator.h>
#include <objects/int_object.h>
#include <objects/list_object.h>
#include <panic.h>

struct _list_node {
  SVM_OBJECT_HEAD;
  list_node_t *next;
  list_node_t *prev;
  svm_object_t *value;
};

static svm_object_type list_node_type;

static list_node_t *list_node_new(svm_object_t *value) {
  list_node_t *node = CREATE_OBJECT(list_node_t, &list_node_type);
  node->next = node->prev = NULL;
  node->value = RETAIN(value);
  return node;
}

static void list_node_traverse(list_node_t *this, traverse_op op, void *args) {
  svm_object_traverse(this->value, op, args);
}

static void list_node_unlink(list_node_t *this) {
  this->next = this->prev = NULL;
  this->value = NULL;
}

static void list_node_destructor(list_node_t *node) {
  if (node->value) {
    RELEASE(node->value);
  }
}

static svm_object_type list_node_type = {
    .m_unlink = (unlink_method)&list_node_unlink,
    .m_traverse = (traverse_method)&list_node_traverse,
    .m_destructor = (desctructor_method)&list_node_destructor};

static void list_node_link(list_node_t *l, list_node_t *r) {
  l->next = CAST_TO(list_node_t, RETAIN(r));
  r->prev = CAST_TO(list_node_t, RETAIN(l));
}

/* list_object section */

list_object_t *list_object_new() {
  list_object_t *list = CREATE_OBJECT(list_object_t, &list_object_type);
  list->tail = list->head = NULL;
  list->size = 0;
  return list;
}

void list_object_push_back(list_object_t *this, svm_object_t *object) {
  list_node_t *node = list_node_new(object);
  if (this->head == NULL) {
    this->head = CAST_TO(list_node_t, RETAIN(node));
    this->tail = CAST_TO(list_node_t, RETAIN(node));
  } else {
    list_node_link(this->tail, node);
    RELEASE(this->tail);
    this->tail = CAST_TO(list_node_t, RETAIN(node));
  }
  ++this->size;
}

static void list_destructor(list_object_t *this) {
  list_node_t *node = this->head;
  while (node != NULL) {
    list_node_t *next = node->next;
    RELEASE(node);
    node = next;
  }
}

static svm_object_t *list_index(list_object_t *this, svm_object_t *index) {
  if (!HAS_TYPE(int_object_type, index)) {
    panic("Attempt to index list object with non-int index");
  }
  uint64_t i = CAST_TO(int_object_t, index)->value;
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

static void list_unlink(list_object_t *this) {
  this->head = this->tail = NULL;
  this->size = 0;
}

static void list_traverse(list_object_t *this, traverse_op op) {
  list_node_t *node = this->head;
  while (node) {
    svm_object_traverse(AS_SVM_OBJECT(node), op, NULL);
    node = node->next;
  }
}

svm_object_type list_object_type = {
    .m_unlink = (unlink_method)&list_unlink,
    .m_traverse = (traverse_method)&list_traverse,
    .m_destructor = (desctructor_method)&list_destructor,
    .m_index = (index_method)&list_index,
};

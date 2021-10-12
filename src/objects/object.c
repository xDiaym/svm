#include <allocator.h>
#include <assert.h>
#include <common.h>
#include <gc.h>
#include <objects/object.h>
#include <panic.h>
#include <stdio.h>
#include <stdlib.h>

static svm_object_t *g_first_object = NULL;
static svm_object_t *g_last_object = NULL;

static void update_global_objects_pointers(svm_object_t *obj) {
  if (g_first_object == NULL) {
    g_first_object = g_last_object = obj;
  } else {
    g_last_object->next = obj;
    g_last_object = obj;
  }
}

static void svm_object_print_type(svm_object_type *type) {
  printf("Type:\n"
         "destructor: %p\n"
         "call: %p\n"
         "index: %p\n"
         "add: %p\n"
         "to_string: %p\n",
         type->m_destructor, type->m_call, type->m_index, type->m_add,
         type->m_to_string);
}

void svm_object_print_debug_info(svm_object_t *object) {
  printf("SVM object at [%p].\n"
         "References: %zu\n"
         "Next object at: [%p]\n",
         object, object->ref_count, object->next);
  svm_object_print_type(object->type);
}

svm_object_t *svm_object_create(svm_object_type *type, size_t object_size) {
  svm_object_t *object = (svm_object_t *)svm_malloc(object_size);
  object->next = NULL;
  object->ref_count = 0;
  object->type = type;
  object->gc_flags = GC_COLLECTABLE;

  update_global_objects_pointers(object);

  return object;
}

svm_object_t *get_first_object() { return g_first_object; }

svm_object_t *retain(svm_object_t *obj) {
  ++obj->ref_count;
  return obj;
}

void svm_object_delete(svm_object_t *this) {
  if (SVM_OBJECT_TYPE(this)->m_destructor != NULL) {
    SVM_OBJECT_TYPE(this)->m_destructor(this);
  }
  svm_free(this);
}

void release(svm_object_t *obj) {
  if (!obj) return;
  --obj->ref_count;
  if (obj->ref_count == 0) {
    svm_object_delete(obj);
  }
}

#define GET_METHOD(object, method_name) SVM_OBJECT_TYPE(object)->m_##method_name
#define GENERATE_BUILTIN_UNARY_OP_IMPL(method)                                 \
  svm_object_t *svm_object_##method(svm_object_t *this) {                      \
    assert(this != NULL);                                                      \
    if (GET_METHOD(this, method) == NULL) {                                    \
      panic("Attempt to call method '%s' on object[%p] without that method.",  \
            STR(method), this);                                                \
    }                                                                          \
    return GET_METHOD(this, method)(this);                                     \
  }

#define GENERATE_BUILTIN_BINARY_OP_IMPL(method)                                \
  svm_object_t *svm_object_##method(svm_object_t *this, svm_object_t *arg) {   \
    assert(this != NULL);                                                      \
    if (GET_METHOD(this, method) == NULL) {                                    \
      panic("Attempt to call method '%s' on object[%p] without that method.",  \
            STR(method), this);                                                \
    }                                                                          \
    return GET_METHOD(this, method)(this, arg);                                \
  }

/**
 * Some preprocessor magic. `METHODS` macro contains all methods name.
 * First argument applies to unary operations, second - to binary.
 */
METHODS(GENERATE_BUILTIN_UNARY_OP_IMPL, GENERATE_BUILTIN_BINARY_OP_IMPL)

void svm_object_traverse(svm_object_t *this, traverse_op op, void *args) {
  assert(this != NULL);
  // If already visited, do not mark due to possible recursion.
  int is_visited = op(this, args);
  if (!is_visited && SVM_OBJECT_TYPE(this)->m_traverse != NULL) {
    SVM_OBJECT_TYPE(this)->m_traverse(this, op, args);
  }
}

void svm_object_unlink(svm_object_t *this) {
  assert(this != NULL);
  if (SVM_OBJECT_TYPE(this)->m_unlink) {
    SVM_OBJECT_TYPE(this)->m_unlink(this);
  }
}

svm_object_t *svm_object_call(svm_object_t *this, svm_object_t **args) {
  assert(this != NULL);
  if (this->type->m_call == NULL) {
    panic("Attempt to call method '%s' on object[%p] without that method.",
          "call", this);
  }
  return this->type->m_call(this, args);
}

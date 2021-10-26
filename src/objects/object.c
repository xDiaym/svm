#include <allocator.h>
#include <assert.h>
#include <common.h>
#include <objects/object.h>
#include <panic.h>
#include <stdio.h>
#include <svm_gc.h>

static void svm_object_print_type(svm_object_type *type) {
  printf("Type <%s>:\n"
         "destructor: %p\n"
         "call: %p\n"
         "index: %p\n"
         "add: %p\n"
         "to_string: %p\n",
         type->name, type->m_destructor, type->m_call, type->m_index,
         type->m_add, type->m_to_string);
}

void svm_object_print_debug_info(svm_object_t *object) {
  const char marked = IS_MARKED(object) ? 'M' : '.';
  const char collectable = IS_COLLECTABLE(object) ? 'C' : '.';
  printf("SVM object at [%p].\n"
         "References: %zu\n"
         "Next object at: [%p]\n"
         "GC flags: [%c%c]\n",
         object, object->ref_count, object->next, collectable, marked);
  svm_object_print_type(object->type);
}

#define TYPENAME_OR_ANON(type) ((type)->name ? (type)->name : "anonymous type")
svm_object_t *safe_cast(svm_object_t *obj, svm_object_type *type) {
  if (!HAS_TYPE(type, obj)) {
    panic("Cannot cast <%s> to <%s>.", TYPENAME_OR_ANON(obj->type),
          TYPENAME_OR_ANON(type));
  }
  return obj;
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

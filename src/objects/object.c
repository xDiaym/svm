#include <assert.h>
#include <common.h>
#include <objects/object.h>
#include <panic.h>
#include <stdio.h>
#include <stdlib.h>

static svm_object *first_object = NULL;
static svm_object *last_object = NULL;

static void update_global_objects_pointers(svm_object *obj) {
  if (first_object == NULL)
    first_object = last_object = obj;
  last_object->next = obj;
  last_object = obj;
}

svm_object *svm_object_create(svm_object_type *type, size_t object_size) {
  svm_object *object = (svm_object *)malloc(object_size);
  object->next = NULL;
  object->ref_count = 0;
  object->type = type;

  update_global_objects_pointers(object);

  return object;
}

svm_object *retain(svm_object *obj) {
  ++obj->ref_count;
  return obj;
}

static void object_delete(svm_object *obj) {
  obj->type->m_destructor(obj);
  // TODO: add custom allocator
  free(obj);
}

void release(svm_object *obj) {
  --obj->ref_count;
  if (obj->ref_count == 0) {
    object_delete(obj);
  }
}

#define GET_METHOD(object, method_name) SVM_OBJECT_TYPE(object)->m_##method_name
#define BUILTIN_METHOD_IMPL_1_ARG(method)                                      \
  svm_object *svm_object_##method(svm_object *this) {                          \
    assert(this != NULL);                                                      \
    if (GET_METHOD(this, method) == NULL) {                                    \
      panic("Attempt to call method '%s' on object[%p] without that method.",  \
            STR(method), this);                                                \
    }                                                                          \
    return GET_METHOD(this, method)(this);                                     \
  }

#define BUILTIN_METHOD_IMPL_2_ARGS(method)                                     \
  svm_object *svm_object_##method(svm_object *this, svm_object* arg) {         \
    assert(this != NULL);                                                      \
    if (GET_METHOD(this, method) == NULL) {                                    \
      panic("Attempt to call method '%s' on object[%p] without that method.",  \
            STR(method), this);                                                \
    }                                                                          \
    return GET_METHOD(this, method)(this, arg);                                \
  }

BUILTIN_METHOD_IMPL_1_ARG(to_string)
BUILTIN_METHOD_IMPL_2_ARGS(add)
BUILTIN_METHOD_IMPL_2_ARGS(index)

svm_object *svm_object_call(svm_object *this, svm_object **args) {
  assert(this != NULL);
  if (this->type->m_call == NULL) {
    panic("Attempt to call method '%s' on object[%p] without that method.",
          "call", this);
  }
  return this->type->m_call(this, args);
}

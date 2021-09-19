#include <assert.h>
#include <object.h>
#include <panic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string_object.h>

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

svm_object *svm_object_call(svm_object *this, svm_object **args) {
  assert(this != NULL);
  if (this->type->m_call == NULL) {
    // TODO: add terminate method
    fprintf(stderr, "Attemp to call non-callable object [%x]", this);
    exit(-1);
  }
  return this->type->m_call(this, args);
}

svm_object *svm_object_add(svm_object *this, svm_object *other) {
  assert(this != NULL);
  if (this->type->m_add == NULL) {
    panic(stderr, "Attemp to call non-callale object [%x]", this);
  }
  return this->type->m_add(this, other);
}

svm_object *svm_object_to_string(svm_object *this) {
  assert(this != NULL);
  if (this->type->m_to_string == NULL) {
    panic(stderr, "Attemp to call non-string-convertable object [%x]", this);
  }
  return this->type->m_to_string(this);
}

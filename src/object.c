#include <object.h>
#include <stdlib.h>

static svm_object *first_object = NULL;
static svm_object *last_object = NULL;

static void update_global_objects_pointers(svm_object *obj) {
  if (first_object == NULL)
    first_object = obj;
  if (last_object != NULL)
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

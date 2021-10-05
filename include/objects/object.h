#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct _svm_object svm_object;

/* List of all operations. */
#define METHODS(unary, binary) binary(add) binary(index) unary(to_string)

typedef int(traverse_op)(svm_object *this);
typedef svm_object *(*unary_op)(svm_object *this);
typedef svm_object *(*binary_op)(svm_object *this, svm_object *other);

typedef void (*unlink_method)(svm_object *this);
typedef void (*traverse_method)(svm_object *this, traverse_op op);
typedef void (*desctructor_method)(svm_object *this);
typedef svm_object *(*call_method)(svm_object *this, svm_object **args);

#define GENERATE_UNARY_METHOD_TYPEDEF(name) typedef unary_op name##_method;
#define GENERATE_BINARY_METHOD_TYPEDEF(name) typedef binary_op name##_method;
METHODS(GENERATE_UNARY_METHOD_TYPEDEF, GENERATE_BINARY_METHOD_TYPEDEF)
#undef GENERATE_UNARY_METHOD_TYPEDEF
#undef GENERATE_BINARY_METHOD_TYPEDEF

typedef struct _svm_object_type {
  /* internal methods */
  traverse_method m_traverse;
  unlink_method m_unlink;
  desctructor_method m_destructor;
  /* special methods */
  call_method m_call;
  index_method m_index;
  add_method m_add;
  to_string_method m_to_string;
} svm_object_type;

typedef struct _svm_object {
  svm_object *next;
  size_t ref_count;
  uint8_t gc_flags;
  svm_object_type *type;
} svm_object;

#define SVM_OBJECT_HEAD svm_object _object

void svm_object_print_debug_info(svm_object *object);

svm_object *svm_object_create(svm_object_type *type, size_t object_size);
#define CREATE_OBJECT(object, type)                                            \
  (object *)svm_object_create(type, sizeof(object))

svm_object *get_first_object();

#define CAST_TO(type, x) ((type *)(x))
#define AS_SVM_OBJECT(x) CAST_TO(svm_object, (x))
#define SVM_OBJECT_TYPE(x) (AS_SVM_OBJECT(x)->type)
#define HAS_TYPE(type, x) (SVM_OBJECT_TYPE(x) == &(type))
#define SAME_TYPE(x, y) (AS_SVM_OBJECT(x)->type == AS_SVM_OBJECT(y)->type)

svm_object *retain(svm_object *obj);
#define RETAIN(x) retain(AS_SVM_OBJECT(x))

void svm_object_delete(svm_object *this);
void release(svm_object *obj);
#define RELEASE(x) release(AS_SVM_OBJECT(x))

#define GENERATE_UNARY_METHOD_DEFINITION(name)                                 \
  svm_object *svm_object_##name(svm_object *this);
#define GENERATE_BINARY_METHOD_DEFINITION(name)                                \
  svm_object *svm_object_##name(svm_object *this, svm_object *arg);
METHODS(GENERATE_UNARY_METHOD_DEFINITION, GENERATE_BINARY_METHOD_DEFINITION)
#undef GENERATE_BINARY_METHOD_DEFINITION
#undef GENERATE_UNARY_METHOD_DEFINITION

/**
 * Traverse objects. Applies `op` to `this`. Call `this->type->m_traverse`,
 * if exists.
 *
 * *ALL* container-types should traverse their elements through this function
 */
void svm_object_traverse(svm_object *this, traverse_op op);
void svm_object_unlink(svm_object *this);
svm_object *svm_object_call(svm_object *this, svm_object **args);

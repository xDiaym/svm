#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct _svm_object svm_object_t;

/* List of all operations. */
#define METHODS(unary, binary) binary(add) binary(index) unary(to_string)

typedef int(traverse_op)(svm_object_t *this);
typedef svm_object_t *(*unary_op)(svm_object_t *this);
typedef svm_object_t *(*binary_op)(svm_object_t *this, svm_object_t *other);

typedef void (*unlink_method)(svm_object_t *this);
typedef void (*traverse_method)(svm_object_t *this, traverse_op op);
typedef void (*desctructor_method)(svm_object_t *this);
typedef svm_object_t *(*call_method)(svm_object_t *this, svm_object_t **args);

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

struct _svm_object {
  svm_object_t *next;
  size_t ref_count;
  uint8_t gc_flags;
  svm_object_type *type;
};

#define SVM_OBJECT_HEAD svm_object_t _object

void svm_object_print_debug_info(svm_object_t *object);

svm_object_t *svm_object_create(svm_object_type *type, size_t object_size);
#define CREATE_OBJECT(object, type)                                            \
  (object *)svm_object_create(type, sizeof(object))

svm_object_t *get_first_object();

#define CAST_TO(type, x) ((type *)(x))
#define AS_SVM_OBJECT(x) CAST_TO(svm_object_t, (x))
#define SVM_OBJECT_TYPE(x) (AS_SVM_OBJECT(x)->type)
#define HAS_TYPE(type, x) (SVM_OBJECT_TYPE(x) == &(type))
#define SAME_TYPE(x, y) (AS_SVM_OBJECT(x)->type == AS_SVM_OBJECT(y)->type)

svm_object_t *retain(svm_object_t *obj);
#define RETAIN(x) retain(AS_SVM_OBJECT(x))

void svm_object_delete(svm_object_t *this);
void release(svm_object_t *obj);
#define RELEASE(x) release(AS_SVM_OBJECT(x))

#define GENERATE_UNARY_METHOD_DEFINITION(name)                                 \
  svm_object_t *svm_object_##name(svm_object_t *this);
#define GENERATE_BINARY_METHOD_DEFINITION(name)                                \
  svm_object_t *svm_object_##name(svm_object_t *this, svm_object_t *arg);
METHODS(GENERATE_UNARY_METHOD_DEFINITION, GENERATE_BINARY_METHOD_DEFINITION)
#undef GENERATE_BINARY_METHOD_DEFINITION
#undef GENERATE_UNARY_METHOD_DEFINITION

/**
 * Traverse objects. Applies `op` to `this`. Call `this->type->m_traverse`,
 * if exists.
 *
 * *ALL* container-types should traverse their elements through this function
 */
void svm_object_traverse(svm_object_t *this, traverse_op op);
void svm_object_unlink(svm_object_t *this);
svm_object_t *svm_object_call(svm_object_t *this, svm_object_t **args);

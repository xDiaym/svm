#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct _svm_object svm_object;

typedef svm_object *(*nullary_op)(svm_object *this);
typedef svm_object *(*binary_op)(svm_object *this, svm_object *other);

// TODO: create more methods
typedef void (*desctructor_method)(svm_object *this);
typedef svm_object *(*call_method)(svm_object *this, svm_object **args);
typedef binary_op index_method;
typedef binary_op add_method;
typedef nullary_op to_string_method;

typedef struct _svm_object_type {
  desctructor_method m_destructor;
  call_method m_call;
  index_method m_index;
  add_method m_add;
  to_string_method m_to_string;
} svm_object_type;

typedef struct _svm_object {
  svm_object *next;
  size_t ref_count;
  svm_object_type *type;
} svm_object;

#define SVM_OBJECT_HEAD svm_object _object

void svm_object_print_debug_info(svm_object* object);

svm_object *svm_object_create(svm_object_type *type, size_t object_size);
#define CREATE_OBJECT(object, type)                                            \
  (object *)svm_object_create(type, sizeof(object))


#define CAST_TO(type, x) ((type *)(x))
#define AS_SVM_OBJECT(x) CAST_TO(svm_object, (x))
#define SVM_OBJECT_TYPE(x) (AS_SVM_OBJECT(x)->type)
#define HAS_TYPE(type, x) (SVM_OBJECT_TYPE(x) == &(type))
#define SAME_TYPE(x, y) (AS_SVM_OBJECT(x)->type == AS_SVM_OBJECT(y)->type)

svm_object *retain(svm_object *obj);
#define RETAIN(type, x) CAST_TO(type, retain(AS_SVM_OBJECT(x)))

void release(svm_object *obj);
#define RELEASE(x) release(AS_SVM_OBJECT(x))

svm_object *svm_object_call(svm_object *this, svm_object **args);
svm_object *svm_object_add(svm_object *this, svm_object *arg);
svm_object *svm_object_index(svm_object *this, svm_object *arg);
svm_object *svm_object_to_string(svm_object *this);

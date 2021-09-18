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

extern svm_object *null_object;

svm_object *svm_object_create(svm_object_type *type, size_t object_size);

svm_object *retain(svm_object *obj);
void release(svm_object *obj);

svm_object *svm_object_call(svm_object *this, svm_object **args);
svm_object *svm_object_add(svm_object *this, svm_object *other);

#define CREATE_OBJECT(object, type)                                            \
  (object *)svm_object_create(type, sizeof(object))

#define AS_SVM_OBJECT(x) ((svm_object *)(x))

#define SVM_OBJECT_HEAD svm_object _object

#define AS_TYPE(type, x) ((type *)x)
#define HAS_TYPE(type, x) (AS_SVM_OBJECT(x)->type == &type)
#define SAME_TYPE(x, y) (AS_SVM_OBJECT(x)->type == AS_SVM_OBJECT(y)->type)

#define NEW(type, x) ((type *)retain(x))
#define DELETE(x) release(AS_SVM_OBJECT(x))

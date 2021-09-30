#include <gc.h>
#include <objects/object.h>

int mark_traverse(svm_object *this) {
  int previous_flags = this->gc_flags;
  this->gc_flags |= GC_MARKED;
  return previous_flags & GC_MARKED;
}

void gc_mark() { svm_object_traverse(get_first_object(), &mark_traverse); }

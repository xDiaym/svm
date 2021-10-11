#include <gc.h>
#include <objects/object.h>

#define G_MARKED() ++g_gc_stat.marked
#define G_DELETED() ++g_gc_stat.deleted

static gc_stat_t g_gc_stat = {
    .marked = 0,
    .deleted = 0
};

int mark_traverse(svm_object_t *this) {
  G_MARKED();

  int previous_flags = this->gc_flags;
  this->gc_flags |= GC_MARKED;
  return previous_flags & GC_MARKED;
}

void gc_mark(svm_object_t *obj) { svm_object_traverse(obj, &mark_traverse); }

size_t gc_sweep() {
  size_t deleted = 0;
  svm_object_t *obj = get_first_object();
  svm_object_t *next = NULL;
  svm_object_t *prev = NULL;

  while (obj) {
    next = obj->next;
    if (!(obj->gc_flags & GC_MARKED)) {
      G_DELETED();
      ++deleted;
      svm_object_unlink(obj);
      svm_object_delete(obj);
    } else {
      obj->gc_flags &= ~GC_MARKED;
      if (prev) {
        prev->next = obj;
      }
      prev = obj;
    }
    obj = next;
  }
  return deleted;
}

gc_stat_t gc_get_global_stat() {
  return g_gc_stat;
}

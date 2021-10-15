#include <gc.h>
#include <objects/object.h>

static gc_stat_t g_gc_stat = {.marked = 0, .deleted = 0};

int mark_traverse(svm_object_t *this, size_t *marked) {
  ++(*marked);

  int previous_flags = this->gc_flags;
  this->gc_flags |= GC_MARKED;
  return previous_flags & GC_MARKED;
}

size_t gc_mark(svm_object_t *obj) {
  size_t marked = 0;
  svm_object_traverse(obj, &mark_traverse, &marked);
  return marked;
}

size_t gc_sweep() {
  size_t deleted = 0;
  svm_object_t *obj = get_first_object();
  svm_object_t *next = NULL;
  svm_object_t *prev = NULL;

  while (obj) {
    next = obj->next;
    if (!(obj->gc_flags & GC_MARKED)) {
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

gc_stat_t gc_round(svm_object_t **objs) {
  gc_stat_t stat = {0, 0};
  while (*objs) {
    stat.marked += gc_mark(*objs);
    ++objs;
  }
  stat.deleted = gc_sweep();

  g_gc_stat.deleted += stat.deleted;
  g_gc_stat.marked += stat.marked;

  return stat;
}

gc_stat_t gc_get_global_stat() { return g_gc_stat; }

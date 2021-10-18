#include <gc.h>
#include <allocator.h>
#include <objects/object.h>
#include <stdio.h>

static gc_stat_t g_gc_stat = {.round = 0,
                              .alive = 0, // TODO: move object creation to GC
                              .marked = 0,
                              .deleted = 0};

static svm_object_t *g_first_object = NULL;
static svm_object_t *g_last_object = NULL;

static void update_global_objects_pointers(svm_object_t *obj) {
  if (g_first_object == NULL) {
    g_first_object = g_last_object = obj;
  } else {
    g_last_object->next = obj;
    g_last_object = obj;
  }
}

svm_object_t *svm_object_create(svm_object_type *type, size_t object_size) {
  g_gc_stat.alive += 1;
  svm_object_t *object = AS_SVM_OBJECT(svm_malloc(object_size));
  object->next = NULL;
  object->ref_count = 0;
  object->type = type;
  object->gc_flags = GC_COLLECTABLE;

  update_global_objects_pointers(object);

  return object;
}

svm_object_t *get_first_object() { return g_first_object; }

int mark_traverse(svm_object_t *this, size_t *marked) {
  ++(*marked);

  uint8_t previous_flags = this->gc_flags;
  this->gc_flags |= GC_MARKED;
  return previous_flags & GC_MARKED;
}

size_t gc_mark(svm_object_t *obj) {
  size_t marked = 0;
  svm_object_traverse(obj, (traverse_op)&mark_traverse, &marked);
  return marked;
}

size_t gc_sweep() {
  size_t deleted = 0;
  svm_object_t *obj = get_first_object();
  svm_object_t *next = NULL;
  svm_object_t *prev = NULL;

  while (obj) {
    next = obj->next;
    if (IS_COLLECTABLE(obj) && !IS_MARKED(obj)) {
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
  gc_stat_t stat = {g_gc_stat.round + 1, g_gc_stat.alive, 0, 0};
  while (*objs) {
    stat.marked += gc_mark(*objs);
    ++objs;
  }
  stat.deleted = gc_sweep();
  stat.alive -= stat.deleted;

  g_gc_stat.deleted += stat.deleted;
  g_gc_stat.marked += stat.marked;
  g_gc_stat.alive = stat.alive;
  ++g_gc_stat.round;

  return stat;
}

void gc_print_stat(gc_stat_t stat) {
  printf("Round #%zu statistic:\n"
         "* Alive:\t%zu\n"
         "* Marked:\t%zu\n"
         "* Deleted:\t%zu\n",
         stat.round, stat.alive, stat.marked, stat.deleted);
}

gc_stat_t gc_get_global_stat() { return g_gc_stat; }

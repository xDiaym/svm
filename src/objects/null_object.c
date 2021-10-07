#include <objects/null_object.h>
#include <objects/string_object.h>

static null_object_t *g_null = NULL;

null_object_t *get_null_object() {
  if (g_null == NULL) {
    g_null = CREATE_OBJECT(null_object_t, &null_object_type);
  }
  return g_null;
}

static string_object_t *to_string() { return string_object_from_c_str("null"); }

svm_object_type null_object_type = {
    .m_to_string = (to_string_method)&to_string,
};

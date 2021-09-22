#include <objects/builtin_function_object.h>
#include <objects/object.h>

builtin_function_object *builtin_function_from_c(const char *name,
                                                 c_api_function func) {
  builtin_function_object *f =
      CREATE_OBJECT(builtin_function_object, &builtin_function_object_type);
  f->name = name;
  f->function = func;
  return f;
}

static svm_object *builtin_function_call(builtin_function_object *this,
                                         svm_object **args) {
  return this->function(args);
}

svm_object_type builtin_function_object_type = {
    .m_call = (call_method)&builtin_function_call};

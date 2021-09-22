#include <objects/int_object.h>
#include <objects/object.h>
#include <objects/string_object.h>
#include <stdio.h>

int main() {
  string_object *str =
      RETAIN(string_object, string_object_from_c_str("Hello world!"));
  int_object *index = RETAIN(int_object, int_object_from_int(7));
  string_object *s = RETAIN(string_object,
             svm_object_index(AS_SVM_OBJECT(str), AS_SVM_OBJECT(index)));

  printf("%s\n", s->data);

  svm_object_print_debug_info(AS_SVM_OBJECT(index));
  RELEASE(s);
  RELEASE(str);
  RELEASE(index);
  return 0;
}

#include <object.h>
#include <stdio.h>
#include <string_object.h>

int main() {
  string_object *s1 = NEW(string_object, string_object_from_c_str("Hello "));
  string_object *s2 = NEW(string_object, string_object_from_c_str("world\n"));
  string_object *x = NEW(string_object, svm_object_add(s1, s2));

  printf("X: %s", x->data);

  DELETE(x);
  DELETE(s1);
  DELETE(s2);
  return 0;
}

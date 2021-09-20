#include <int_object.h>
#include <object.h>
#include <stdio.h>
#include <string_object.h>

int main() {
  string_object *str =
      NEW(string_object, string_object_from_c_str("Hello world!"));
  int_object *index = NEW(int_object, int_object_from_int(7));
  string_object *s = NEW(string_object, svm_object_index(str, index));

  printf("%s", s->data);

  DELETE(s);
  DELETE(str);
  DELETE(index);
  return 0;
}

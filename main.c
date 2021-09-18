#include <object.h>
#include <panic.h>
#include <stdio.h>
#include <string_object.h>

int main() {
  string_object *s1 = NEW(string_object, string_object_from_c_str("Hello "));
  string_object *s2 = NEW(string_object, string_object_from_c_str("world\n"));

  DELETE(s1);
  DELETE(s2);
  return 0;
}

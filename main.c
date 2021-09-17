#include <object.h>
#include <stdio.h>
#include <string_object.h>

int main() {
  string_object *string =
      NEW(string_object, string_object_from_c_str("Hello world!\n"));
  printf(string->data);
  DELETE(string);
  return 0;
}

#include <int_object.h>
#include <object.h>
#include <stdio.h>
#include <string_object.h>

int main() {
  int_object *x = NEW(int_object, int_object_from_int(-42));
  string_object *y = NEW(string_object, svm_object_to_string(AS_SVM_OBJECT(x)));
  printf("%s\n", y->data);
  DELETE(y);
  DELETE(x);
  return 0;
}

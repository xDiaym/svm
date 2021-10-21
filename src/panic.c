#include <panic.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void panic(const char *message, ...) {
  va_list args;
  va_start(args, message);
  vfprintf(stderr, message, args);
  va_end(args);
  exit(-1);
}

void svm_assert(const char *file, size_t line, const char *condition,
                const char *message) {
  fprintf(stderr,
          "[%s:%zu] Assertion failed!\n"
          "%s is false",
          file, line, condition);
  fprintf(stderr, "%s", message);
  exit(-1);
}

#include <panic.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void panic(const char *message, ...) {
  va_list args;
  va_start(args, message);
  vfprintf(stderr, message, args);
  exit(-1);
}

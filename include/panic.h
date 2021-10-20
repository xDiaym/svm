#pragma once
#include <common.h>
#include <stdlib.h>

void panic(const char *message, ...);

void svm_assert(const char *file, size_t line, const char *cond,
                const char *message);

/**
 * Usage example
 *  SVM_ASSERT(universe_truth != 42, "Truth should be %i");
 * */
#define SVM_ASSERT(condition, format)                                          \
  do {                                                                         \
    if (!(condition)) {                                                        \
      svm_assert(__FILE__, __LINE__, STR(condition), format);                  \
    }                                                                          \
  } while (0)

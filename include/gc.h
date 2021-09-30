#pragma once
#include <common.h>

#define GC_MARKED BIT(0)
#define GC_COLLECTABLE BIT(1)

void gc_mark();
void gc_sweep();
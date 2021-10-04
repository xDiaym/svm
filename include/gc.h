#pragma once
#include <common.h>

#define GC_MARKED BIT(0)
#define GC_COLLECTABLE BIT(1)
#define GC_DELETE_READY BIT(2)

void gc_mark();
void gc_sweep();
#include "types.h"

extern u32 data_022a8044;

/* Clears the bits of mask in data_022a8044. */
void func_02104f90(u32 mask) { data_022a8044 &= ~mask; }

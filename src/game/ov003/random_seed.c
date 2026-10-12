#include "types.h"

extern u32 gOv003RandState;

void Ov003_RandSeed(u32 seed) { gOv003RandState = seed; }

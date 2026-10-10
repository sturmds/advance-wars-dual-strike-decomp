#include "types.h"

void Ov003Unit_SpawnFacingEffect(const void *src, void *dst, u32 size);

void Ov003Unit_FireForward(const void *src, void *dst, u32 size)
{
    Ov003Unit_SpawnFacingEffect(src, dst, size);
}

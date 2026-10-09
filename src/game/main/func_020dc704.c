#include "types.h"

void Sound_RestoreSongVolume(const void *src, void *dst, u32 size);

void func_020dc704(const void *src, void *dst, u32 size)
{
    Sound_RestoreSongVolume(src, dst, size);
}

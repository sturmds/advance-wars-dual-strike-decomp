#include "sdk/fs.h"

void Mem_CopyBytes(const void* src, void* dest, u32 size);

int FSi_WriteMemCallback(FSArchive* arc, const void* src, u32 dest, u32 len) {
  Mem_CopyBytes(src, (void*)(arc->base + dest), len);
  return FS_RESULT_SUCCESS;
}

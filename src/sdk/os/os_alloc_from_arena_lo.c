#include "types.h"
#include "sdk/os.h"

void* OS_GetArenaLo(int id);
void* OS_GetArenaHi(int id);
void OS_SetArenaLo(int id, void* lo);

/* Takes size bytes from the bottom of arena id, aligned to align. Returns NULL if it does not fit. */
void* OS_AllocFromArenaLo(int id, u32 size, u32 align) {
  u32 start;
  u32 end;
  u32 newLo;
  u32 oldLo = (u32)OS_GetArenaLo(id);

  if (oldLo == 0) {
    return NULL;
  }
  start = (oldLo + align - 1) & ~(align - 1);
  end = start + size;
  newLo = (end + align - 1) & ~(align - 1);
  if (newLo > (u32)OS_GetArenaHi(id)) {
    return NULL;
  }
  OS_SetArenaLo(id, (void*)newLo);
  return (void*)start;
}

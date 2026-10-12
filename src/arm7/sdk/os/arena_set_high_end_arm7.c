#include "types.h"
#include "sdk/os_arm7.h"

void Arena_SetHighEnd_Arm7(int id, void* hi) {
  ARENA_BOUNDS_TABLE->hi[id] = hi;
}

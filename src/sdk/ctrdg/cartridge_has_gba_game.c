#include "types.h"
#include "sdk/ctrdg.h"

BOOL Cartridge_HasGbaGame(void) {
  GbaSlotBootSnapshot* info = GBA_SLOT_SNAPSHOT_PTR;

  return info->isAgb;
}

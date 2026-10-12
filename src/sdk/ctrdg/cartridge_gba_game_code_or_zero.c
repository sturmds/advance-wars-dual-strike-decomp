#include "types.h"
#include "sdk/ctrdg.h"

u32 Cartridge_GbaGameCodeOrZero(void) {
  u32 gameCode = 0;
  GbaSlotBootSnapshot* info = GBA_SLOT_SNAPSHOT_PTR;

  if (Cartridge_HasGbaGame()) {
    gameCode = info->gameCode;
  }
  return gameCode;
}

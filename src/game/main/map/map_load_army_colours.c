#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gArmyColours[ARMY_SLOT_COUNT];

/* Copies the current map's army colour per slot into gArmyColours. */
void Map_LoadArmyColours(void) {
  Unk_02080440* game = data_027c027c;
  int index = data_027c0284;
  int slot;
  u8* dst = gArmyColours;

  for (slot = 0; slot < ARMY_SLOT_COUNT; slot++) {
    *dst++ = Ov000_gMapHeaders[game->unk_48[index]->unk_1c4->mapId].army[slot];
  }
}

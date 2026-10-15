#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

/* For each player slot 1-4, ORs the current map header's two flag bytes for
 * that slot into the player, unless the first is 0xff. */
void Map_ApplySlotFlags(void) {
  int slot;
  u8 flags;

  for (slot = 1; slot < ARMY_SLOT_COUNT; slot++) {
    flags =
        Ov000_gMapHeaders[data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId]
            .slotFlags[slot]
            .unk_0;
    if (flags != 0xff) {
      data_027c027c->unk_48[data_027c0284]->players[slot].hqX |= flags;
      data_027c027c->unk_48[data_027c0284]->players[slot].hqY |=
          Ov000_gMapHeaders[data_027c027c->unk_48[data_027c0284]
                                ->unk_1c4->mapId]
              .slotFlags[slot]
              .unk_1;
    }
  }
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/owner_info.h"
#include "game/player_panel_cache.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0280* data_027c0280;
u8 data_ov000_023503a0;
extern const void* const data_ov000_022f6610[];
extern u8 data_0213f12c[];

#define STATE (data_027c027c->unk_48[data_027c0284])

void TransferQueue_Add(const void* src, void* dst, u32 size);
void Palette_LoadArmyObjRow(int player, int slot);

/* Queues two graphics blocks for object VRAM (the first is picked by the byte
 * in data_ov000_023503a0), then loads each active player's army palette into
 * sprite palette slot player + 10. */
void Ov000_LoadObjGfx(void) {
  int player;

  TransferQueue_Add(data_ov000_022f6610[data_ov000_023503a0],
                    data_027c0280->objVram, 0x900);
  TransferQueue_Add(data_0213f12c, data_027c0280->objVram + 0x1100, 0xc00);
  for (player = 1; player < 5; player++) {
    if (STATE->players[player].controller != PLAYER_CONTROLLER_NONE) {
      Palette_LoadArmyObjRow(player, player + 10);
    }
  }
}

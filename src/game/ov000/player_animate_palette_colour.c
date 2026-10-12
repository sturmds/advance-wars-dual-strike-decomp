#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 data_0213fdcc[16];
extern u16 data_0213fdec[16];

#define STATE (data_027c027c->unk_48[data_027c0284])

int MainLoop_GetFrameCount(void);
BOOL CanUseCoPower(int player, int slot);
void Palette_LoadWithCopy(const void* src, u16 offset, u32 size);

/* Cycles colour 15 of sprite palette slot player + 10 (palette offset
 * (player + 26) * 32 + 30) through 16 colours, one step every 4 frames. The
 * colour table depends on whether the player can use a CO power. Does nothing
 * unless GameState.unk_5c is set. The volatile frame count is match-only: the
 * original keeps it on the stack. */
void Player_AnimatePaletteColour(int player) {
  volatile u32 frame = MainLoop_GetFrameCount();

  if (STATE->unk_5c == 0) {
    return;
  }
  if (CanUseCoPower(player, 0)) {
    Palette_LoadWithCopy(&data_0213fdcc[(frame >> 2) & 15],
                         ((player + 26) << 5) + 30, 2);
  } else {
    Palette_LoadWithCopy(&data_0213fdec[(frame >> 2) & 15],
                         ((player + 26) << 5) + 30, 2);
  }
}

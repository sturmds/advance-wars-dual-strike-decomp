#include "types.h"
#include "game/Unk_02080440.h"

void* Mem_Alloc(u32 size);
void Mem_Free(void* block);
void Units_CopyBlockToPlayer(MapUnit* block, int player);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Moves the units of player slots 1-4 to slots player1-player4 (their
 * MAP_UNITS_PER_PLAYER blocks in state->units). The one caller passes four
 * state bytes at 0x8e-0x91 after loading a saved game. */
void Units_ReassignPlayerSlots(int player1, int player2, int player3,
                               int player4) {
  MapUnit* copy;
  int i;

  if (player1 == 1 && player2 == 2 && player3 == 3 && player4 == 4) {
    return;
  }
  copy = Mem_Alloc(4 * MAP_UNITS_PER_PLAYER * sizeof(MapUnit));
  for (i = 0; i < 4 * MAP_UNITS_PER_PLAYER; i++) {
    copy[i] = STATE->units[i];
  }
  Units_CopyBlockToPlayer(&copy[0 * MAP_UNITS_PER_PLAYER], player1);
  Units_CopyBlockToPlayer(&copy[1 * MAP_UNITS_PER_PLAYER], player2);
  Units_CopyBlockToPlayer(&copy[2 * MAP_UNITS_PER_PLAYER], player3);
  Units_CopyBlockToPlayer(&copy[3 * MAP_UNITS_PER_PLAYER], player4);
  Mem_Free(copy);
}

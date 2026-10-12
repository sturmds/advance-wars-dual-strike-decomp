#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7c5c.h"

void GfxQueue_AddKind2(const void* src, void* dst, u32 size);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 data_02139450[];

void func_020f8534(int player, int index) {
  Player* p = &data_027c027c->unk_48[data_027c0284]->players[player];
  Unk_022a7c5c* gfx = data_022a7c5c;
  u32 offset = (p->armyColour - 1) * 32 + 30;

  GfxQueue_AddKind2(&data_02139450[index], gfx->unk_34 + offset, 2);
}

#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

void Mem_Zero32(u32 value, void* dst, u32 size);
void Palette_Flush(void);
void Ov004_InitVramBanks(void);

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern void* data_ov004_022bf14c;

void Ov004Task_ResetState(Ov004Task* task) {
  Unk_ov004_022bf158* state = task->state;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  Mem_Zero32(0, data_ov004_022bf158, 0x1c10);
  Ov004_InitVramBanks();
  state->unk_000 = 0;
  state->unk_004 = 0;
  state->unk_008 = 0;
  state->buffer = data_ov004_022bf14c;
  state->vramArea = 0x06860000;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Mem_Zero32(0, data_027c0280->paletteBuffer, 0x400);
  Palette_Flush();
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Mem_Zero32(0, data_027c0280->paletteBuffer, 0x400);
  Palette_Flush();
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}

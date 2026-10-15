#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Mem_Fill32(u32 value, void* dst, u32 size);
void Palette_Load(const void* src, int offset, u32 size);

extern u8 data_021388f0[];

/* Task init of HelpBand_Start, on the lower screen. The band's sprites are
 * OBJ window sprites, so it fills their 8 tiles at unk_52 with solid pixels
 * and turns the OBJ window on: inside it only layer 0 shows, without colour
 * effects; outside, every layer and the sprites show. Then loads BG palette
 * 1 from data_021388f0. */
void HelpBand_Init(Task* task) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Mem_Fill32(0xffffffff, data_027c0280->objVram + task->unk_52 * 32, 0x100);
  GFX_DISPCNT(data_027c0280)->objWin = 1;
  data_027c0280->objWinIn.effect = 0;
  data_027c0280->objWinIn.bg[data_027c0280->layer0Bg] = 1;
  data_027c0280->objWinIn.bg[data_027c0280->layer1Bg] = 0;
  data_027c0280->objWinIn.bg[data_027c0280->layer2Bg] = 0;
  data_027c0280->objWinIn.bg[data_027c0280->layer3Bg] = 0;
  data_027c0280->objWinIn.obj = 0;
  data_027c0280->winOut.bg[data_027c0280->layer0Bg] = 1;
  data_027c0280->winOut.bg[data_027c0280->layer1Bg] = 1;
  data_027c0280->winOut.bg[data_027c0280->layer2Bg] = 1;
  data_027c0280->winOut.bg[data_027c0280->layer3Bg] = 1;
  data_027c0280->winOut.obj = 1;
  data_027c0280->winOut.effect = 1;
  Palette_Load(data_021388f0, 32, 32);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}

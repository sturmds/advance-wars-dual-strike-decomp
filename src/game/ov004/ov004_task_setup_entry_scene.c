#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"

void Screen_InitLayers(const GfxLayer* config);
void Screen_SetBgMode(int bgMode);
void Ov004_ClearSlots(Unk_ov004_022bf158* state);

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern GfxLayer data_ov004_022be958[];

/* An entry starts hidden; width and height hold the end values of
 * Ov004_EntryAppear's eases. */
#define INIT_ENTRY(entry, slot, startShade, startX) \
  (entry)->active = 0;                              \
  (entry)->texSlot = (slot);                        \
  (entry)->shade = (startShade);                    \
  (entry)->x = (startX);                            \
  (entry)->y = -64;                                 \
  (entry)->width = 64;                              \
  (entry)->height = 128

/* Script step: sets up the main screen's BG layers (BG0-BG2 shown, no
 * windows or blending) and places seven hidden entries in a row, 32 apart. */
void Ov004Task_SetupEntryScene(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_ov004_022be958);
  data_027c0280->dispCnt |= 0x100; /* BG0-BG2 on, BG3 and OBJ off */
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt &= ~0x1000;
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->dispCnt &= ~0x2000; /* windows off */
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  INIT_ENTRY(&state->entries[5], 6, 31, -128);
  INIT_ENTRY(&state->entries[3], 3, 26, -96);
  INIT_ENTRY(&state->entries[1], 1, 21, -64);
  INIT_ENTRY(&state->entries[0], 0, 16, -32);
  INIT_ENTRY(&state->entries[2], 2, 11, 0);
  INIT_ENTRY(&state->entries[4], 4, 6, 32);
  INIT_ENTRY(&state->entries[6], 5, 1, 64);
  state->unk_8f8 = 230;
  state->unk_8fc = -32;
  state->unk_900 = 0;
  Ov004_ClearSlots(state);
  state->bg3InSecondHalf = 0;
  state->bgCharBase = 0;
  task->frame = 0;
  task->duration = 0;
  task->step = 0;
}

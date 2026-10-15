#include "types.h"
#include "game/menu_state.h"
#include "game/task.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"

u8* File_Load(const char* path);
void Mem_Free(void* res);
void Data_Unpack(const void* src, void* dest);
void Mem_Copy32(const void* src, void* dest, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void Gfx_RequestLayerCopy(int layer);

extern char gTagBreakThunderPath[]; /* "/ohashi/res_tagbreak_thunder" */

#define LAYER_TILES(layerBg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[layerBg].charBase << 14))

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Bytes at 0x280c of the thunder file that go to TagBreakWork.palettes. */
#define THUNDER_SPRITE_DATA_SIZE 0x40

/* Step of the tag break script: loads res_tagbreak_thunder (layer 0 tiles,
 * a sprite file, the spark map and palettes for TagBreak_StartSparks, and
 * sprite tiles with their palette), resets layer 0's scroll, alpha-blends
 * BG0 over BG1-BG3 and OBJ at full weight, and starts an ease task under
 * this one from 31 to 15 over 32 frames. */
void TagBreak_LoadThunder(Task* task) {
  u8* res;

  res = File_Load(gTagBreakThunderPath);
  Data_Unpack(res, LAYER_TILES(data_027c0280->layer0Bg));
  Data_Unpack(res + 0x2254, gMenuState.bytes);
  Data_Unpack(res + 0x252c, gMenuState.tagBreak.layer0Map);
  Mem_Copy32(res + 0x280c, gMenuState.tagBreak.palettes,
             THUNDER_SPRITE_DATA_SIZE);
  Data_Unpack(res + 0x284c, data_027c0280->objVram + 0x3280);
  Palette_Load(res + 0x28a8, 0x260, 0x20);
  Mem_Free(res);
  Gfx_RequestLayerCopy(0);
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 1;
  FRAME(task) = 0;
  EaseTask_Start(31, 15, 32, task);
}

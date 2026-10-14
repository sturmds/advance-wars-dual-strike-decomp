#include "types.h"
#include "game/map_select.h"
#include "game/menu_button.h"
#include "game/Unk_02088d04.h"

void Gfx_RequestLayerCopy(int layer);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_Flush(void);
void Screen_SetCurrentGfx(void* state);
void MapSelect_LoadEnemyCoPictures(void);

extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

#define GFX data_027c0280
/* BG tile data of a BG, 32 bytes per tile. */
#define BG_CHARS(bg) (GFX->bgVram + GFX->bgCnt[bg].charBase * 0x4000)
/* Maps on one page of the list. */
#define PAGE_MAPS (ROWS * COLS)
/* First tile of the map thumbnails: they fill the last PAGE_MAPS * 32
 * of the 1024 tiles. */
#define THUMB_TILE (1024 - PAGE_MAPS * 32)

/* Needed to match: through an inline function the compiler reads the BG
 * number before the last ROWS and COLS, as the original does. */
static inline int Gfx_LayerBg(int layer) {
  return GFX_LAYER(data_027c0280, layer)->bg;
}

/* Starts the slide between two map lists (MapSelect_NextList,
 * MapSelect_PrevList). Copies the first half of the thumbnail tiles to the
 * second half, and the first 32 by 32 tile maps of layer 0 and, on the other
 * screen, layer 1 to the second ones, moving their tile numbers to the
 * copies. Then scrolls both layers to the copies (x = 256), and keeps the
 * War Room records shown so far in prevRecordCos, prevRecordDays and prevRecordScores. */
void MapSelect_StartListSlide(int slideBack) {
  Unk_027c0280* savedGfx;
  int i;
  int j;

  MENU->setup.slideFrames = 16;
  MENU->setup.slideBack = slideBack;
  MenuButtons_RemoveSelectable();
  MenuButton_DisableTouch(254);
  MenuButton_DisableTouch(252);
  MenuButton_DisableTouch(253);
  MenuButton_DisableTouch(251);
  MenuButton_DisableTouch(250);
  MapSelect_LoadEnemyCoPictures();

  Mem_Copy32(BG_CHARS(Gfx_LayerBg(0)) + THUMB_TILE * 32,
             BG_CHARS(Gfx_LayerBg(0)) + (PAGE_MAPS * 16 + THUMB_TILE) * 32,
             PAGE_MAPS * 512);
  Mem_Copy32(GFX->layer0Map, GFX->layer0Map + BG_SCREEN_ENTRIES,
             BG_SCREEN_SIZE);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    if ((GFX->layer0Map[BG_SCREEN_ENTRIES + i] & 0x3ff) >= THUMB_TILE) {
      GFX->layer0Map[BG_SCREEN_ENTRIES + i] += PAGE_MAPS * 16;
    }
  }
  Gfx_RequestLayerCopy(0);
  GFX->bgScrollX[GFX->layer0Bg] = 256;
  Mem_Copy32(GFX->objVram + 0x3940, GFX->objVram + 0x4240, 0x900);
  Mem_Copy32(GFX->paletteBuffer + 0x260, GFX->paletteBuffer + 0x320, 0xc0);
  Palette_Flush();

  for (i = 0; i < (int)ARRAY_COUNT(MENU->setup.prevRecordDays); i++) {
    MENU->setup.prevRecordCos[i * 2] = MENU->setup.recordCos[i * 2];
    MENU->setup.prevRecordCos[i * 2 + 1] = MENU->setup.recordCos[i * 2 + 1];
    MENU->setup.prevRecordDays[i] = MENU->setup.recordDays[i];
    MENU->setup.prevRecordScores[i] = MENU->setup.recordScores[i];
  }

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Mem_Copy32(BG_CHARS(GFX->layer1Bg) + 0x2000, BG_CHARS(GFX->layer1Bg) + 0x2200,
             0x200);
  Mem_Copy32(GFX->layer1Map, GFX->layer1Map + BG_SCREEN_ENTRIES,
             BG_SCREEN_SIZE);
  for (j = 0; j < BG_VISIBLE_ENTRIES; j++) {
    if (GFX->layer1Map[BG_SCREEN_ENTRIES + j] & 0x3ff) {
      GFX->layer1Map[BG_SCREEN_ENTRIES + j] += 16;
    }
  }
  Gfx_RequestLayerCopy(1);
  GFX->bgScrollX[GFX->layer1Bg] = 256;
  Mem_Copy32(GFX->objVram + 0x8000, GFX->objVram + 0xc000, 0x4000);
  Screen_SetCurrentGfx(savedGfx);
}

#include "types.h"
#include "game/player.h"
#include "game/page_strip.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02088d04.h"

u32 HwMod32(int num, int denom);
int MainLoop_GetFrameCount(void);
int Map_GetPlayerCount(int mapId);
void Palette_Load(const void* src, int offset, int size);
void Screen_SetCurrentGfx(void* state);
void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);
void Sprite_QueuePicture192x160(s16 x, int y, int a2, int a3, int a4);
void Co_QueueSmallPictureSpriteAtTile(int x, int y, int attr, int tile, int a4);
void Co_QueueSmallPictureSprite(int x, int y, int attr, int a3);
void SurvivalSelect_DrawPageCount(int pages);
int SurvivalSelect_GetList(void);
int SurvivalRecord_GetCo(int mode, int a1);
int SurvivalRecord_Exists(int mode);
int Survival_GetMapId(int mode, int index);

extern Unk_022a7b80 data_022a7b80;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_027c0028;
extern u8 data_027c0040;
extern u8 data_027c0048;
extern u8 data_027c00b8;
extern u8 data_027c0108;
extern u8 data_027c01c0;

/* Draws one frame of the Survival map select. Top screen: one tab per entry of
 * the current list (the selected one 32 pixels wide, the others 16) with a
 * link piece between tabs; when the list has 2 or more pages (unk_03), the
 * entries before unk_02 use palette 9. Then two cycling palette colours and
 * one marker per player for the selected map's player count. Bottom screen:
 * a list label and, when SurvivalRecord_Exists reports it, two more icons. */
void SurvivalSelect_Draw(void) {
  int mode = data_022a7b80.mode;
  int list = SurvivalSelect_GetList();
  int playerCount = Map_GetPlayerCount((u16)Survival_GetMapId(
      data_022a7b80.mode, data_0229c744->lists[list].cursor));
  Unk_027c0280* savedState = data_027c0280;
  int col;
  int x;
  int i;
  int j;
  int pal;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  x = 0;
  col = 0;
  for (i = 0; i < data_0229c744->lists[list].count; i++) {
    pal = 8;
    if (data_0229c744->lists[list].unk_03 >= 2) {
      if (i < HwMod32(data_0229c744->lists[list].unk_02,
                      data_0229c744->lists[list].count)) {
        pal = 9;
      } else {
        pal = 8; /* needed to match: makes mwcc load 8 before 9 */
      }
    }
    if (i == data_0229c744->lists[list].cursor) {
      Sprite_QueueAt(6, x & 0x1ff, 0, &data_027c0048, (pal << 12) | 0x800,
                     col * 16 + 0x82);
      x += 32;
    } else {
      Sprite_QueueAt(6, x & 0x1ff, 8, &data_027c0040, (pal << 12) | 0x800,
                     0x172 + col * 4);
      x += 16;
    }
    if (data_0229c744->lists[list].unk_03 >= 2) {
      if (i < HwMod32(data_0229c744->lists[list].unk_02 - 1,
                      data_0229c744->lists[list].count)) {
        pal = 9;
      } else {
        pal = 8;
      }
    }
    if (i != data_0229c744->lists[list].count - 1) {
      Sprite_QueueAt(5, (x - 3) & 0x1ff, 16, &data_027c0028,
                     (pal << 12) | 0x800, 128);
      col++;
      if (col == 15) {
        col = 0;
      }
    }
  }
  SurvivalSelect_DrawPageCount(data_0229c744->lists[list].unk_03);
  Palette_Load(&data_0229c744->colours[(MainLoop_GetFrameCount() >> 2) & 15],
               0x318, 2);
  Palette_Load(&data_0229c744->colours[(MainLoop_GetFrameCount() >> 2) & 15],
               0x338, 2);
  Sprite_QueueAt(0, 104, 136, &data_027c0108, 0x8000, 96);
  /* Needed to match: the bound is written as <= count - 1. */
  for (j = 0; j <= PLAYER_CO_COUNT - 1; j++) {
    Co_QueueSmallPictureSpriteAtTile(0x10c0, (168 + j * 16) & 0xff,
                                     (j + 6) << 12, 72 + j * 12, j & 1);
  }
  if (data_0229c744->lists[SurvivalSelect_GetList()].count != 0) {
    for (i = 0; i < playerCount - 1; i++) {
      /* Needed to match: the bound is written as <= count - 1. */
      for (j = 0; j <= PLAYER_CO_COUNT - 1; j++) {
        int k = j + i * 2;
        Co_QueueSmallPictureSpriteAtTile((24 + j * 32) & 0x1ff,
                                         (192 - i * 16) & 0xff, k << 12, k * 12,
                                         k & 1);
      }
    }
    Sprite_QueueAt(0, 0, (192 - playerCount * 16) & 0xff, &data_027c00b8,
                   0xa000, 112);
    Sprite_QueuePicture192x160(128, 96, 0xb800, 0x400, 7);
  }
  Screen_SetCurrentGfx(savedState);

  savedState = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  if (list == 0) {
    Sprite_QueueAt(0, 64, 86, &data_027c01c0, 0x4018, 0);
  } else {
    Sprite_QueueAt(0, 64, 86, &data_027c01c0, 0x4038, 0);
  }
  if (SurvivalRecord_Exists(mode) == 1) {
    Co_QueueSmallPictureSprite(48, 136, 0x2000, 0);
    if (SurvivalRecord_GetCo(mode, 1) != 0) {
      Co_QueueSmallPictureSprite(80, 136, 0x300c, 1);
    }
    Sprite_QueueAt(0, 216, 120, &data_027c0040, 0x5058, 0);
  }
  Screen_SetCurrentGfx(savedState);
}

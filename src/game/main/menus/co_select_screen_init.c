#include "types.h"
#include "game/icon.h"
#include "game/co_select_screen.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

/* CO in grid cell (column 1-4, row 0-1). Needed to match: coGrid read as rows of 2. */
#define GRID_CO(column, row) \
  (((u8(*)[2])data_0229c73c->coGrid)[(column) - 1][row])

int Campaign_IsHardMode(void);
void Co_UnpackPicture(int coId, int firstTile);
void Data_Unpack(const void* src, void* dest);
u8* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void ForceRank_LoadSets(int mode);
void Gfx_LoadBmap09f(void* a, int b, int c);
void Gfx_RequestLayerCopy(int bg);
void MapPreview_DrawPlayerViews(u8 (*dst)[32]);
void Mem_Free(void* res);
void Mem_Zero32(u32 value, void* dest, u32 size);
void MenuButton_Select(int button);
void MenuButtons_DisableTouch(void);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
void Menu_Reset(void);
void ModeFont_Load(int x, int y);
void ModeFont_ShowTextOnScreen(const char* text, int tile, int paletteSlot,
                               int unk_56, u8 unk_5c, int unk_58);
void PaletteCycle_Start(int a);
void Palette_Load(const void* src, int offset, u32 size);
void Palette_LoadObjRow(int row);
void Screen_SetCurrentBoth(void* state);
BOOL Skills_AreDisabled(void);
const void* Text_Get(u32 textId);
void Wallpaper_LoadSelected(void);
void Window_DrawOnLayer1(int x, int y, int width, int height);
void NumberFont_LoadLow(void);
void Co_LoadGraphicsAndPalette(int co, int tile, int row);
void func_02028808(void);
void func_02028978(void);
void TpIcon_Load(void);
void func_0202b7e0(int a0);
void func_0202b804(void* dest);
void CoSelectScreen_AddButtons(void);
void CoSelectScreen_RefreshCellSkills(u8 column, u8 row);
void CoSelectScreen_ClearCellSkills(void);
void CoSelectScreen_DrawSkillSlots(void);
void CoSelectScreen_LoadListTextures(void);
int CoSelectScreen_RowTileY(int a0);
int CoSelectScreen_ColumnTileX(u8 column);
void CoSelectScreen_DrawColumnFrame(u8* src, u8 a1, u8 a2, u8 column);

extern char data_0215ab90[]; /* "/ohashi/res_syogunselect" */
extern char data_0215abac[]; /* "/ohashi/res_syogunselect_lang" */

/* Sets up both screens of the CO select screen from the res_syogunselect
 * files. Top: the background (one layout in a normal-mode campaign, another
 * otherwise), a frame per player column (two frame sets, by row count) and
 * the picture of each CO in the grid. Bottom: the COs of the cursor column
 * (an empty row is cleared), player and number icons and the map preview.
 * Then loads the Force Rank sets per grid cell unless skills are off, shows
 * text 0x7d000004 and resets the menu.
 * Needed to match: the picture loop and the Force Rank loop each have their
 * own counters. */
void CoSelectScreen_Init(void) {
  u8* res;
  u8* langRes;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  Unk_027c0280* savedTop;
  Unk_027c0280* savedBottom;
  int column;
  int row;
  int i;
  int frameOffset;
  int pictureColumn;
  int pictureRow;
  int rankColumn;
  int rankRow;

  func_02028808();
  func_02028978();
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  GFX_DISPCNT(data_027c0280)->objMap1D = 1;
  data_027c0280->unk_dd = 1;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  GFX_DISPCNT(data_027c0280)->objMap1D = 1;
  data_027c0280->unk_dd = 1;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  res = File_Load(data_0215ab90);
  langRes = File_Load(File_LocalizePath(data_0215abac));

  savedTop = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Data_Unpack(
      langRes,
      data_027c0280->bgVram +
          data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase * 0x4000 +
          0x4000);
  if (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN && Campaign_IsHardMode() == 0) {
    Data_Unpack(res + 0x224, data_027c0280->layer1Map);
  } else {
    Data_Unpack(res + 0x3a0, data_027c0280->layer1Map);
  }
  Palette_Load(res + 0x51c, 0x40, 0x60);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer1Map[i] += 0x2200;
  }
  frameOffset = data_0229c73c->rowCount == 2 ? 0x57c : 0x5f0;
  for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
    CoSelectScreen_DrawColumnFrame(res + frameOffset,
                                   CoSelectScreen_ColumnTileX(column),
                                   CoSelectScreen_RowTileY(0), column);
  }
  Gfx_RequestLayerCopy(1);
  Data_Unpack(res, data_027c0280->objVram);
  Palette_Load(res + 0x204, 0x200, 32);
  for (pictureColumn = 1; pictureColumn < data_0229c73c->columnCount + 1;
       pictureColumn++) {
    for (pictureRow = 0; pictureRow < data_0229c73c->rowCount; pictureRow++) {
      Co_UnpackPicture(GRID_CO(pictureColumn, pictureRow),
                       ((pictureColumn - 1) * 2 + pictureRow) * 12 + 0x2c);
    }
  }
  Palette_LoadObjRow(1);
  Screen_SetCurrentGfx(savedTop);

  savedBottom = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Window_DrawOnLayer1(3, 18, 26, 6);
  CoSelectScreen_DrawSkillSlots();
  for (row = 0; row < data_0229c73c->rowCount; row++) {
    if (GRID_CO(data_0229c73c->cursorColumn, row) == 0) {
      /* 0x1b0 tiles of 32 bytes per row; needed to match: indexed by tile */
      Mem_Zero32(0, (u8(*)[32])data_027c0280->objVram + row * 0x1b0, 0x3600);
    } else {
      Co_LoadGraphicsAndPalette(GRID_CO(data_0229c73c->cursorColumn, row),
                                row * 0x1b0, row);
    }
  }
  for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
    Icon_LoadTo(column + 74, data_027c0280->objVram, (column - 1) * 8 + 0x360);
  }
  Icon_LoadTo(139, data_027c0280->objVram, 0x378);
  for (i = 0; i < ICON_DIGIT_COUNT; i++) {
    Icon_LoadTo(ICON_DIGITS + i, data_027c0280->objVram, i * 2 + 0x380);
  }
  Icon_LoadTo(213, data_027c0280->objVram, i * 2 + 0x380);
  IconBank_LoadPalette(1, 18);
  IconBank_LoadPalette(4, 19);
  func_0202b804(data_027c0280->objVram + 0x8b00);
  func_0202b7e0(21);
  MapPreview_DrawPlayerViews((u8(*)[32])(data_027c0280->objVram + 0x7300));
  Screen_SetCurrentGfx(savedBottom);
  Mem_Free(res);
  Mem_Free(langRes);

  CoSelectScreen_LoadListTextures();
  CoSelectScreen_ClearCellSkills();
  if (!Skills_AreDisabled()) {
    ForceRank_LoadSets(MAP_STATE->gameMode);
    for (rankColumn = 1; rankColumn < data_0229c73c->columnCount + 1;
         rankColumn++) {
      for (rankRow = 0; rankRow < data_0229c73c->rowCount; rankRow++) {
        CoSelectScreen_RefreshCellSkills(rankColumn, rankRow);
      }
    }
  }
  NumberFont_LoadLow();
  TpIcon_Load();
  ModeFont_Load(0x5d8, 6);
  ModeFont_ShowTextOnScreen(Text_Get(0x7d000004), 0x5d8, 6, 0x800, 7, 0);
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 256, -16);
  MenuButton_Select(0);
  Wallpaper_LoadSelected();

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  PaletteCycle_Start(20);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  CoSelectScreen_AddButtons();
  MenuButtons_DisableTouch();
}

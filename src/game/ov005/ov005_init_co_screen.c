#include "types.h"
#include "game/task.h"
#include "game/unit_show_task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba8.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Mem_Copy32(const void* src, void* dst, u32 size);
BOOL CoSelect_IsCoAvailable(u8 coId);
s16 func_ov005_02362bf0(Unk_ov005_0236bba8* work, int index);
int Co_GetSavedPaletteVariant(int coId);
void Ov005_ReadCoPaletteVariant(Unk_ov005_0236bba8* work);
void func_02028978(void);
void Wallpaper_LoadSelected(void);
void TpIcon_Load(void);
void ModeFont_Load(int x, int y);
const char* Text_Get(u32 textId);
void ModeFont_ShowTextOnScreen(const char* text, int tile, int paletteSlot,
                               int unk_56, u8 unk_5c, int unk_58);
void HelpBand_Start(int a, int b);
void HelpText_Show(u32 textId, int tile);
void Gfx_LoadBmap09f(void* target, int a, int b);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void* File_Load(const char* path);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Ov005_LoadCoGraphicsToBothScreens(Unk_ov005_0236bba8* work);
void Ov005_UnpackCoScreenPicture(Unk_ov005_0236bba8* work);
void IconBanks_ClearSlots(void);
void IconBank_Init(int bank, u8* vram, int firstTile, int paletteSlot);
void Icon_Load(int iconId);
void Ov005_LoadCoGridTextures(Ov005Task* task);
ShowSprite* SpriteAnim_Create(void* file, int a, int b);
void SpriteAnim_SetAnimation(ShowSprite* sprite, int frame);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const Ov005CoGridEntry data_ov005_0236ac90[OV005_CO_GRID_ENTRIES];
extern const u8 data_ov005_0236ac24[0x6c];
extern const TaskScriptCmd data_ov005_0236ab6c[];
extern char data_ov005_0236ae50[]; /* "/rikiishi" */
extern char data_ov005_0236ae5c[]; /* "03a": main screen layer 1 tiles */
extern char data_ov005_0236ae60[]; /* "03b": main screen layer 1 map */
extern char data_ov005_0236ae64[]; /* "03c": main screen layer 1 palette */
extern char data_ov005_0236ae74[]; /* "03d": sub screen layer 1 map */
extern char data_ov005_0236ae78[]; /* "041": main screen OBJ tiles */
extern char data_ov005_0236ae7c[]; /* "043": main screen OBJ palette */
extern char data_ov005_0236ae80[]; /* "042": sprite file */
extern u32 data_ov005_02367fcc[];  /* map bytes of a text BG by screen size */
extern u32 data_ov005_02367fdc
    []; /* map bytes of a rotation/scaling BG by screen size */

/* First step of the CO picture screen (after its work block is allocated):
 * copies the 4 by 7 CO grid and marks each cell available or not, puts the
 * cursor on cell 0, loads the sub screen's title text, background and icons,
 * the main screen's layer 1 background and the CO picture, starts the child
 * task data_ov005_0236ab6c on the same work block, and makes nine sprites from
 * rikiishi file 042. */
void Ov005_InitCoScreen(Ov005Task* task) {
  Unk_ov005_0236bba8* work = task->work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int i;

  Mem_Copy32(data_ov005_0236ac90, work->grid, sizeof(work->grid));
  Mem_Copy32(data_ov005_0236ac24, work->paletteButtons,
             sizeof(work->paletteButtons));
  for (i = 0; i < OV005_CO_GRID_ENTRIES; i++) {
    if (work->grid[i].coId == -1) {
      work->grid[i].available = 0;
    } else if (CoSelect_IsCoAvailable(work->grid[i].coId) == 0) {
      work->grid[i].available = 0;
    } else {
      work->grid[i].available = 1;
    }
  }

  work->cursor = 0;
  work->prevCursor = 0;
  work->cursorRow = 0;
  work->cursorCoId = func_ov005_02362bf0(work, work->cursor);
  work->coId = work->cursorCoId;
  work->unk_2c = Co_GetSavedPaletteVariant(work->coId);
  work->unk_28 = work->unk_2c;
  work->unk_1fc = 0xc8000;
  work->unk_200 = 0xa0000;
  work->unk_204 = 0xa0000;
  Ov005_ReadCoPaletteVariant(work);
  func_02028978();
  Wallpaper_LoadSelected();
  TpIcon_Load();

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  ModeFont_Load(0, 0);
  ModeFont_ShowTextOnScreen(Text_Get(0x7d000002), 0, 0, 0x800, 10, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  HelpBand_Start(0x3f8, 1);
  HelpText_Show(0x80000035, 64);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  FS_ChangeDir(data_ov005_0236ae50);
  File_UnpackTo(data_027c0280->layer1Map, data_ov005_0236ae74);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  /* needed to match: the layer 1 loading gets its own locals */
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;
    u16* map;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    FS_ChangeDir(data_ov005_0236ae50);
    File_UnpackTo(
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14),
        data_ov005_0236ae5c);
    File_UnpackTo(data_027c0280->layer1Map, data_ov005_0236ae60);
    map = data_027c0280->layer1Map;
    TileMap_AddToEntries(
        map,
        Screen_IsTextBg(1)
            ? data_ov005_02367fcc[GFX_LAYER(data_027c0280, 1)->screenSize]
        : Screen_IsAffineBg(1)
            ? data_ov005_02367fdc[GFX_LAYER(data_027c0280, 1)->screenSize]
            : data_ov005_02367fdc[GFX_LAYER(data_027c0280, 1)->screenSize] * 2,
        0);
    Palette_LoadFromFile(data_ov005_0236ae64, 0, 0, 0xa0);
    Gfx_RequestLayerCopy(1);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  Ov005_LoadCoGraphicsToBothScreens(work);
  Ov005_UnpackCoScreenPicture(work);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  IconBanks_ClearSlots();
  IconBank_Init(1, data_027c0280->objVram, 0x180, 20);
  Icon_Load(75);
  Icon_Load(76);
  Icon_Load(77);
  Icon_Load(78);
  Icon_Load(79);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  Ov005_LoadCoGridTextures(task);
  work->childTask = (Ov005Task*)Task_Create(data_ov005_0236ab6c, task);
  work->childTask->work = task->work;

  /* needed to match: the sprite loading gets its own locals */
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;
    int sprite;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    FS_ChangeDir(data_ov005_0236ae50);
    File_UnpackTo(data_027c0280->objVram, data_ov005_0236ae78);
    Palette_LoadFromFile(data_ov005_0236ae7c, 0, 0x200, 32);
    work->spriteFile = File_Load(data_ov005_0236ae80);
    for (sprite = 0; sprite < 9; sprite++) {
      work->unk_21c[sprite] = SpriteAnim_Create(work->spriteFile, 0, 2);
      ((ShowSprite*)work->unk_21c[sprite])->unk_24 = 0;
      SpriteAnim_SetAnimation(work->unk_21c[sprite], sprite);
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  work->unk_00 = 0;
  task->step = 0;
}

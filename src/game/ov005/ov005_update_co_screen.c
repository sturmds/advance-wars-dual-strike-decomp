#include "types.h"
#include "game/keypad.h"
#include "game/ov005_task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bba8.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void func_020048f4(u16 frames);
void func_0200493c(u16 frames);
void func_02004984(u16 frames);
void func_020049cc(u16 frames);
int ScreenTransition_IsRunning(void);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
void MenuButtons_EnableTouch(void);
void MenuButtons_DisableTouch(void);
void MenuButtons_RemoveSelectable(void);
void MenuButtons_RemoveUnselectable(void);
void Sound_PlaySe(u32 soundId);
void HelpText_Show(u32 textId, int tile);
void Ov005_ReadCoPaletteVariant(Unk_ov005_0236bba8* work);
int Co_GetPictureVariant(int coId);
void Co_SetPictureVariant(int coId, int variant);
void Co_LoadGraphics(int coId, int firstTile);
void func_02002804(int coId, int firstTile);
void Co_SetSavedPaletteVariant(int coId, int variant);
int Co_GetRank(u32 co);
void SpriteAnim_Draw(void* sprite, int x, int y);
void Task_Destroy(void* task);
void Task_ResumeScript(void* task);
void Wallpaper_DestroyTasks(void);
void Screen1_DestroyTextTasks(void);
void HelpBand_Stop(void);
void Screens_BlackOut(void);
void EventSequenceTask_Start(Ov005Task* task);
void Ov005_DrawCoGridCells(Ov005Task* task);
void Ov005_CopyCoPaletteToBothScreens(Unk_ov005_0236bba8* work);
void Ov005_AddCoPaletteButtons(Ov005Task* task);
void Ov005_InitCoPaletteButtons(Ov005Task* task);
void func_ov005_02362cf8(Ov005Task* task);
void func_ov005_02362d68(Ov005Task* task);
void Ov005_AddCoGridButtons(Ov005Task* task);
void Ov005_LoadCoGridTextures(Ov005Task* task);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_ov005_0236ae50[]; /* "/rikiishi" */
extern char data_ov005_0236ae5c[]; /* "03a": grid layer 1 tiles */
extern char data_ov005_0236ae60[]; /* "03b": grid layer 1 map */
extern char data_ov005_0236ae64[]; /* "03c": grid layer 1 palette */
extern char data_ov005_0236ae68[]; /* "03e": palette screen layer 1 tiles */
extern char data_ov005_0236ae6c[]; /* "03f": palette screen layer 1 map */
extern char data_ov005_0236ae70[]; /* "040": palette screen layer 1 palette */
/* map bytes of a text BG by screen size */
const u32 data_ov005_02367fcc[] = {
#include "data/ov005/data_ov005_02367fcc.inc.c"
};
/* map bytes of a rotation/scaling BG by screen size */
const u32 data_ov005_02367fdc[] = {
#include "data/ov005/data_ov005_02367fdc.inc.c"
};

#define SE_CONFIRM 0x10006
#define SE_CANCEL 0x10009
#define SE_MOVE 0x10002

/* Palette variant 8 swaps the CO's picture instead of its colours. */
#define CO_PICTURE_SWAP_VARIANT 8

/* Loads layer 1 of the main screen from /rikiishi: tiles, map and the
 * first `paletteSize` bytes of palette. */
#define LOAD_LAYER1(tiles, mapFile, palette, paletteSize)                     \
  saved0280 = data_027c0280;                                                  \
  Screen_SetCurrentGfx(data_027c0288.unk_00);                                 \
  saved027c = data_027c027c;                                                  \
  Screen_SetCurrent(data_027c0288.unk_00);                                    \
  FS_ChangeDir(data_ov005_0236ae50);                                          \
  File_UnpackTo(                                                              \
      data_027c0280->bgVram +                                                 \
          (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14),     \
      tiles);                                                                 \
  File_UnpackTo(data_027c0280->layer1Map, mapFile);                           \
  map = data_027c0280->layer1Map;                                             \
  TileMap_AddToEntries(                                                       \
      map,                                                                    \
      Screen_IsTextBg(1)                                                      \
          ? data_ov005_02367fcc[GFX_LAYER(data_027c0280, 1)->screenSize]      \
      : Screen_IsAffineBg(1)                                                  \
          ? data_ov005_02367fdc[GFX_LAYER(data_027c0280, 1)->screenSize]      \
          : data_ov005_02367fdc[GFX_LAYER(data_027c0280, 1)->screenSize] * 2, \
      0);                                                                     \
  Palette_LoadFromFile(palette, 0, 0, paletteSize);                           \
  Gfx_RequestLayerCopy(1);                                                    \
  Screen_SetCurrent(saved027c);                                               \
  Screen_SetCurrentGfx(saved0280)

/* Shows text `textId` on the sub screen. */
#define SHOW_SUB_TEXT(textId)                 \
  saved0280 = data_027c0280;                  \
  Screen_SetCurrentGfx(data_027c0288.unk_04); \
  saved027c = data_027c027c;                  \
  Screen_SetCurrent(data_027c0288.unk_04);    \
  HelpText_Show(textId, 0x3c0);               \
  Screen_SetCurrent(saved027c);               \
  Screen_SetCurrentGfx(saved0280)

/* Per-frame update of the CO picture screen (first step:
 * Ov005_InitCoScreen). Steps 0-20: fade in and pick a CO on the grid.
 * A goes to the palette buttons (50-60), where A applies the variant under
 * the cursor and B goes back to the grid (10-14). B on the grid fades out,
 * tears the screen down (100-102), and step 103 starts EventSequenceTask_Start if a
 * variant was applied, then resumes the task script. Every frame it draws
 * the grid cells or the palette sprites and plays the move sound when the
 * cursor or the variant changed. */
void Ov005_UpdateCoScreen(Ov005Task* task) {
  Unk_ov005_0236bba8* work = task->work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u16* map;

  switch (task->step) {
    case 0:
      func_020048f4(32);
      work->paletteMode = 0;
      task->step++;
      break;
    case 1:
      if (ScreenTransition_IsRunning() != 1) {
        task->step++;
      }
      break;
    case 2:
      MenuCursor_SetRect(0, -16, 256, -16);
      Ov005_AddCoGridButtons(task);
      func_ov005_02362d68(task);
      task->step = 20;
      break;
    case 10:
      func_020049cc(32);
      task->step++;
      break;
    case 11:
      if (ScreenTransition_IsRunning() != 1) {
        task->step++;
      }
      break;
    case 12:
      work->paletteMode = 0;
      LOAD_LAYER1(data_ov005_0236ae5c, data_ov005_0236ae60, data_ov005_0236ae64,
                  0xa0);
      data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
      data_027c0280->dispCnt |= 0x100;
      data_027c0280->dispCnt |= 0x200;
      data_027c0280->dispCnt |= 0x400;
      data_027c0280->dispCnt |= 0x800;
      data_027c0280->dispCnt |= 0x1000;
      Ov005_LoadCoGridTextures(task);
      SHOW_SUB_TEXT(0x80000035);
      MenuButtons_RemoveUnselectable();
      func_ov005_02362d68(task);
      MenuButtons_DisableTouch();
      task->step++;
      break;
    case 13:
      func_02004984(32);
      work->gridConfirmTouched = 0;
      work->paletteConfirmTouched = 0;
      work->backTouched = 0;
      task->step++;
      break;
    case 14:
      if (ScreenTransition_IsRunning() != 1) {
        MenuButtons_EnableTouch();
        Ov005_AddCoGridButtons(task);
        task->step = 20;
      }
      break;
    case 20:
      if ((gKeypad->pressed & PAD_BUTTON_A) || work->gridConfirmTouched == 1) {
        MenuButtons_RemoveSelectable();
        MenuButtons_DisableTouch();
        Sound_PlaySe(SE_CONFIRM);
        task->step = 50;
      } else if ((gKeypad->pressed & PAD_BUTTON_B) || work->backTouched == 1) {
        MenuButtons_RemoveSelectable();
        MenuButtons_DisableTouch();
        Sound_PlaySe(SE_CANCEL);
        task->step = 100;
      }
      break;
    case 50:
      func_020049cc(32);
      task->step++;
      break;
    case 51:
      if (ScreenTransition_IsRunning() != 1) {
        task->step++;
      }
      break;
    case 52:
      work->paletteMode = 1;
      Ov005_ReadCoPaletteVariant(work);
      LOAD_LAYER1(data_ov005_0236ae68, data_ov005_0236ae6c, data_ov005_0236ae70,
                  32);
      SHOW_SUB_TEXT(0x80000036);
      data_027c0280->bgScrollY[data_027c0280->layer1Bg] = -24;
      data_027c0280->dispCnt &= ~0x100;
      data_027c0280->dispCnt |= 0x200;
      data_027c0280->dispCnt |= 0x400;
      data_027c0280->dispCnt |= 0x800;
      data_027c0280->dispCnt |= 0x1000;
      MenuButtons_RemoveSelectable();
      Ov005_InitCoPaletteButtons(task);
      MenuButtons_RemoveUnselectable();
      func_ov005_02362cf8(task);
      MenuButtons_DisableTouch();
      task->step++;
      break;
    case 53:
      func_02004984(32);
      task->step++;
      break;
    case 54:
      if (ScreenTransition_IsRunning() != 1) {
        MenuButtons_EnableTouch();
        Ov005_AddCoPaletteButtons(task);
        task->step = 60;
      }
      break;
    case 60:
      if (work->variant != CO_PICTURE_SWAP_VARIANT &&
          work->prevVariant == CO_PICTURE_SWAP_VARIANT) {
        SHOW_SUB_TEXT(0x80000036);
      }
      if (work->variant == CO_PICTURE_SWAP_VARIANT &&
          work->prevVariant != CO_PICTURE_SWAP_VARIANT) {
        SHOW_SUB_TEXT(0x80000037);
      }
      if ((gKeypad->pressed & PAD_BUTTON_A) ||
          work->paletteConfirmTouched == 1) {
        work->paletteConfirmTouched = 0;
        if (work->variant == CO_PICTURE_SWAP_VARIANT) {
          Co_SetPictureVariant(
              work->coId & 0x7f,
              (Co_GetPictureVariant(work->coId & 0x7f) + 1) & 1);
          saved0280 = data_027c0280;
          Screen_SetCurrentGfx(data_027c0288.unk_04);
          saved027c = data_027c027c;
          Screen_SetCurrent(data_027c0288.unk_04);
          Co_LoadGraphics(work->coId, 0x200);
          Screen_SetCurrent(saved027c);
          Screen_SetCurrentGfx(saved0280);
          saved0280 = data_027c0280;
          Screen_SetCurrentGfx(data_027c0288.unk_00);
          saved027c = data_027c027c;
          Screen_SetCurrent(data_027c0288.unk_00);
          func_02002804(work->coId, 0x200);
          Screen_SetCurrent(saved027c);
          Screen_SetCurrentGfx(saved0280);
        } else {
          work->unk_28 = work->variant;
          Co_SetSavedPaletteVariant(work->coId, work->unk_28);
          Ov005_CopyCoPaletteToBothScreens(work);
        }
        work->unk_00 = 1;
        Sound_PlaySe(SE_CONFIRM);
      } else if ((gKeypad->pressed & PAD_BUTTON_B) || work->backTouched == 1) {
        work->backTouched = 0;
        MenuButtons_RemoveSelectable();
        MenuButtons_DisableTouch();
        Sound_PlaySe(SE_CANCEL);
        task->step = 10;
      }
      break;
    case 100:
      func_0200493c(32);
      task->step++;
      break;
    case 101:
      if (ScreenTransition_IsRunning() != 1) {
        task->step++;
      }
      break;
    case 102:
      Task_Destroy(work->childTask);
      Wallpaper_DestroyTasks();
      Screen1_DestroyTextTasks();
      HelpBand_Stop();
      MenuButtons_RemoveUnselectable();
      Screens_BlackOut();
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->dispCnt &= ~0x100;
      data_027c0280->dispCnt &= ~0x200;
      data_027c0280->dispCnt &= ~0x400;
      data_027c0280->dispCnt &= ~0x800;
      data_027c0280->dispCnt &= ~0x1000;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      data_027c0280->dispCnt &= ~0x100;
      data_027c0280->dispCnt &= ~0x200;
      data_027c0280->dispCnt &= ~0x400;
      data_027c0280->dispCnt &= ~0x800;
      data_027c0280->dispCnt &= ~0x1000;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      task->step++;
      break;
    case 103:
      if (work->unk_00) {
        EventSequenceTask_Start(task);
      }
      Task_ResumeScript(task);
      return;
  }

  if (work->paletteMode == 0) {
    Ov005_DrawCoGridCells(task);
  } else {
    /* needed to match: the sprite drawing gets its own locals */
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;
    int i;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    for (i = 0; i < 9; i++) {
      /* the picture swap button only shows from rank 10 */
      if (Co_GetRank(work->coId) >= 10 || i != CO_PICTURE_SWAP_VARIANT) {
        SpriteAnim_Draw(work->unk_21c[i], work->extraX[i] - 1, work->extraY[i]);
      }
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  if (work->cursor != work->prevCursor) {
    Sound_PlaySe(SE_MOVE);
  }
  if (work->variant != work->prevVariant) {
    Sound_PlaySe(SE_MOVE);
  }
  work->prevCursor = work->cursor;
  work->prevVariant = work->variant;
}

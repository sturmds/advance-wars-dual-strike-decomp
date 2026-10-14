/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void Gfx_ClearLayerMap(int layer);
void Wallpaper_LoadSingleOnFirstScreen(void);
void Font_LoadDigits(int a);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);
void Gfx_LoadBmap09f(void* target, int a, int b);
int NameEntry_GetTextLength(NameEntry* entry);
void TpIcon_Load(void);
void NameEntry_LoadKeyTable(NameEntry* entry);
void NameEntry_LoadCaretSprite(NameEntry* entry);
void NameEntry_DrawKeyLabels(NameEntry* entry);
void NameEntry_LoadButtonSprites(NameEntry* entry);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern GfxLayer data_0216ac74[];
extern char data_0216b338[];
extern char data_0216b344[];
extern u8 gNameEntryText[];
extern u8 gNameEntryStartText[];

/* Needed to match: an inline call evaluates its arguments (the layer
 * number, the map pointer) before the body reads data_027c0280 again. */
static inline void SetBgPriority(int bg, int priority) {
  data_027c0280->bgCnt[bg].priority = priority;
}

static inline void FillMapRect(u16* map, int x, int y, int width, int height,
                               int tile) {
  Bg_FillRect(map, x, y, width, height, tile);
}

static inline void SetBgScrollX(int bg, int x) {
  data_027c0280->bgScrollX[bg] = x;
}

static inline void SetBgScrollY(int bg, int y) {
  data_027c0280->bgScrollY[bg] = y;
}

/* Opens the name entry screen (reached through a function table, next to
 * NameEntry_AllocWork): sets up screen 0 (data_027c0288.unk_00)
 * for a text field of task->maxLength characters (8 or 16), draws the field's
 * frame, resets the work block and loads the keyboard and its sprites. */
void NameEntry_SetUpScreen(NameEntryTask* task) {
  NameEntry* work = task->work;
  u16* dst;
  u16* src;
  int j;

  work->maxLength = task->maxLength;

  {
    Unk_027c0280* savedGfx = data_027c0280;
    Unk_02080440* savedScreen;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Screen_SetBgMode(0);
    if (work->maxLength == 8) {
      Screen_InitLayers(data_0216ac74);
    } else {
      Gfx_ClearLayerMap(0);
      Gfx_ClearLayerMap(1);
      Gfx_ClearLayerMap(2);
    }
    GFX_DISPCNT(data_027c0280)->bg0 = 1;
    GFX_DISPCNT(data_027c0280)->bg1 = 1;
    GFX_DISPCNT(data_027c0280)->bg2 = 0;
    GFX_DISPCNT(data_027c0280)->bg3 = 1;
    GFX_DISPCNT(data_027c0280)->obj = 1;
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
  }

  {
    Unk_027c0280* savedGfx = data_027c0280;
    Unk_02080440* savedScreen;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    SetBgPriority(data_027c0280->layer0Bg, 0);
    SetBgPriority(data_027c0280->layer1Bg, 1);
    SetBgPriority(data_027c0280->layer2Bg, 2);
    SetBgPriority(data_027c0280->layer3Bg, 3);
    GFX_DISPCNT(data_027c0280)->win0 = 0;
    GFX_DISPCNT(data_027c0280)->win1 = 0;
    GFX_DISPCNT(data_027c0280)->objWin = 0;
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    GFX_DISPCNT(data_027c0280)->objMap1D = 1;
    data_027c0280->unk_dd = 0;
    SetBgScrollY(data_027c0280->layer0Bg, 0);
    SetBgScrollX(data_027c0280->layer0Bg, 0);
    SetBgScrollY(data_027c0280->layer1Bg, 0);
    SetBgScrollX(data_027c0280->layer1Bg, 0);
    SetBgScrollY(data_027c0280->layer2Bg, 0);
    SetBgScrollX(data_027c0280->layer2Bg, 0);
    SetBgScrollY(data_027c0280->layer3Bg, 0);
    SetBgScrollX(data_027c0280->layer3Bg, 0);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
  }

  if (work->maxLength == 8) {
    Wallpaper_LoadSingleOnFirstScreen();
  }

  {
    Unk_027c0280* savedGfx = data_027c0280;
    Unk_02080440* savedScreen;
    int i;
    int middleTiles;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    src = work->frameTiles[0];
    dst = data_027c0280->layer1Map;
    Font_LoadDigits(0);
    FS_ChangeDir(data_0216b338);
    File_UnpackTo(data_027c0280->layer1Map, data_0216b344);
    Mem_Copy32(data_027c0280->layer1Map, work->frameTiles,
               sizeof(work->frameTiles));
    FillMapRect(data_027c0280->layer1Map, 0, 0, 32, 4, 0);
    /* Draw the text field frame centred in rows 0-3: the left edge,
         * one middle column per character, then the right edge. */
    if (work->maxLength == 8) {
      dst += 11;
      middleTiles = 8;
    }
    if (work->maxLength == 16) {
      dst += 7;
      middleTiles = 16;
    }
    dst[0] = src[0];
    dst[32] = src[32];
    dst[64] = src[64];
    dst[96] = src[96];
    dst++;
    for (i = 0; i < middleTiles; i++) {
      dst[0] = src[1];
      dst[32] = src[33];
      dst[64] = src[65];
      dst[96] = src[97];
      dst++;
    }
    dst[0] = src[2];
    dst[32] = src[34];
    dst[64] = src[66];
    dst[96] = src[98];
    Gfx_RequestLayerCopy(1);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
  }

  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  work->cursor = 0;
  work->lastColumn = 0;
  work->prevCursor = 0;
  work->unk_11c = 0;
  work->unk_120 = 0;
  work->unk_124 = 0;
  work->unk_128 = 0;
  work->unk_12c = 0;
  work->unk_130 = 0;
  work->unk_114 = NameEntry_GetTextLength(work);
  work->textLength = work->unk_114;
  if (work->textLength >= work->maxLength * 2) {
    work->textLength = work->maxLength * 2 - 1;
  }
  work->scriptRunning = 0;

  {
    Unk_027c0280* savedGfx = data_027c0280;
    Unk_02080440* savedScreen;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    TpIcon_Load();
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
  }

  NameEntry_LoadKeyTable(work);
  NameEntry_LoadCaretSprite(work);
  NameEntry_DrawKeyLabels(work);
  NameEntry_LoadButtonSprites(work);

  /* Clear everything after the first zero byte, then keep a copy of
     * the starting text. ended is block-scoped to match: it keeps a
     * register and middleTiles goes on the stack. */
  {
    int ended = FALSE;

    for (j = 0; j < NAME_ENTRY_TEXT_SIZE; j++) {
      if (!ended) {
        if (gNameEntryText[j] == 0) {
          ended = TRUE;
        }
      } else {
        gNameEntryText[j] = 0;
      }
    }
  }
  Mem_Copy32(gNameEntryText, gNameEntryStartText, NAME_ENTRY_TEXT_SIZE);
  task->step = 0;
}

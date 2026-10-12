#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/brightness_fade.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern u32 data_ov004_022bdc94[]; /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84
    []; /* map bytes of a rotation/scaling BG by screen size */
extern TaskScriptCmd data_ov004_022be418[];

extern char data_ov004_022bdd60[];
extern char data_ov004_022bdd64[];
extern char data_ov004_022bdd84[];
extern char data_ov004_022bdd8c[];
extern char data_ov004_022bdd90[];
extern char data_ov004_022bdd98[];
extern char data_ov004_022bdde0[];
extern char data_ov004_022bdde4[];
extern char data_ov004_022bdde8[];
extern char data_ov004_022bddf0[];
extern char data_ov004_022bddf4[];
extern char data_ov004_022bde08[];

void Task_ResumeScript(void* task);
void func_ov004_022bb1d8(Unk_ov004_022bf158* state);
void Ov004_SetSlotTask(Unk_ov004_022bf158* state, void* task, int id);
void Ov004_UpdateElapsedFrames(Unk_ov004_022bf158* state);
int Ov004_CheckTimedEvent(Unk_ov004_022bf158* state, int event, int frame);
void Ov004_StartClock(Unk_ov004_022bf158* state);
void Ov004_UnpackResource(const char* name, void* dst);
void Ov004_CopyResource(const char* name, void* dst);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);
void* PaletteFade_StartFromBlack(int a, void* b, int c, int d, int e);
void* PaletteFade_StartLevels(int a, int b, int c, int d, int e, int f);
void* PaletteFade_StartToBlack(int a, int b, int c, int d);
void Palette_Darken(u8* palettes, int first, int count, int level);

/* Byte size of layer i's tile map. */
#define LAYER_MAP_BYTES(i)                                            \
  (Screen_IsTextBg(i)                                                 \
       ? data_ov004_022bdc94[GFX_LAYER(data_027c0280, i)->screenSize] \
   : Screen_IsAffineBg(i)                                             \
       ? data_ov004_022bdc84[GFX_LAYER(data_027c0280, i)->screenSize] \
       : data_ov004_022bdc84[GFX_LAYER(data_027c0280, i)->screenSize] * 2)

/* Tile data of BG bg (BG VRAM plus its character base). */
#define BG_CHAR_DATA(bg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[bg].charBase << 14))

/* Loads a picture into layer 2 on the current screen: tiles, map (entries
 * get palette 1) and palette 1, then darkens BG palettes 1-8 by 4/16. */
#define LOAD_LAYER2_PICTURE(tiles, mapName, palette)                    \
  do {                                                                  \
    Unk_027c0280* savedGfx;                                             \
    Unk_02080440* savedState;                                           \
    u16* map;                                                           \
                                                                        \
    savedGfx = data_027c0280;                                           \
    Screen_SetCurrentGfx(data_027c0288.unk_00);                         \
    savedState = data_027c027c;                                         \
    Screen_SetCurrent(data_027c0288.unk_00);                            \
    Ov004_UnpackResource(tiles, BG_CHAR_DATA(data_027c0280->layer2Bg)); \
    Ov004_UnpackResource(mapName, GFX_LAYER(data_027c0280, 2)->map);    \
    Ov004_CopyResource(palette, data_027c0280->paletteBuffer + 0x20);   \
    map = GFX_LAYER(data_027c0280, 2)->map;                             \
    TileMap_AddToEntries(map, LAYER_MAP_BYTES(2), 0x1000);              \
    Gfx_RequestLayerCopy(2);                                            \
    Palette_Flush();                                                    \
    Palette_Darken(data_027c0280->paletteBuffer, 1, 8, 4);              \
    Screen_SetCurrent(savedState);                                      \
    Screen_SetCurrentGfx(savedGfx);                                     \
  } while (0)

/* PaletteFade_StartLevels, PaletteFade_StartToBlack and PaletteFade_StartFromBlack wrap PaletteFade_Start, which
 * makes a task that changes BG palettes over time with Palette_Darken. */
#define START_PALETTE_FADE_TASK()                                          \
  do {                                                                     \
    int id = task->duration++;                                             \
    Ov004_SetSlotTask(state, PaletteFade_StartLevels(5, 16, 1, 16, 4, 16), \
                      id);                                                 \
  } while (0)

/* Runs a timeline of cues (Ov004_CheckTimedEvent, each fires once): at frame 0
 * it starts three tasks, then loads four pictures into layer 2 of the current
 * screen (frames 108, 136, 164 and 192), each followed ten frames later by a
 * palette fade task, and resumes the script at frame 223.
 * task->duration counts the task slots handed out. */
void Ov004Task_RunPictureTimeline(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  Ov004_UpdateElapsedFrames(state);
  if (Ov004_CheckTimedEvent(state, 0, 0)) {
    int id;

    id = task->duration++;
    Ov004_SetSlotTask(
        state, PaletteFade_StartFromBlack(5, state->savedPalette, 30, 0, 1),
        id);
    id = task->duration++;
    Ov004_SetSlotTask(state, Task_Create(data_ov004_022be418, TASK_LIST(5)),
                      id);
    id = task->duration++;
    Ov004_SetSlotTask(state, func_020fcda0(TASK_LIST(5), 32), id);
  }
  if (Ov004_CheckTimedEvent(state, 1, 108)) {
    LOAD_LAYER2_PICTURE(data_ov004_022bdd84, data_ov004_022bddf0,
                        data_ov004_022bdd98);
  }
  if (Ov004_CheckTimedEvent(state, 2, 118)) {
    START_PALETTE_FADE_TASK();
  }
  if (Ov004_CheckTimedEvent(state, 3, 136)) {
    LOAD_LAYER2_PICTURE(data_ov004_022bdde0, data_ov004_022bddf4,
                        data_ov004_022bdd64);
  }
  if (Ov004_CheckTimedEvent(state, 4, 146)) {
    START_PALETTE_FADE_TASK();
  }
  if (Ov004_CheckTimedEvent(state, 5, 164)) {
    LOAD_LAYER2_PICTURE(data_ov004_022bdde4, data_ov004_022bdd90,
                        data_ov004_022bdd60);
  }
  if (Ov004_CheckTimedEvent(state, 6, 174)) {
    START_PALETTE_FADE_TASK();
  }
  if (Ov004_CheckTimedEvent(state, 7, 192)) {
    LOAD_LAYER2_PICTURE(data_ov004_022bdde8, data_ov004_022bdd8c,
                        data_ov004_022bde08);
  }
  if (Ov004_CheckTimedEvent(state, 8, 202)) {
    START_PALETTE_FADE_TASK();
  }
  if (Ov004_CheckTimedEvent(state, 9, 190)) {
    int id = task->duration++;
    Ov004_SetSlotTask(state, PaletteFade_StartToBlack(5, 28, 0, 1), id);
  }
  if (Ov004_CheckTimedEvent(state, 10, 223)) {
    Ov004_StartClock(state);
    func_ov004_022bb1d8(state);
    Task_ResumeScript(task);
  }
}

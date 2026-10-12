#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/brightness_fade.h"

void Ov004_UpdateElapsedFrames(Unk_ov004_022bf158* state);
int Ov004_CheckTimedEvent(Unk_ov004_022bf158* state, int event, int frame);
void Ov004_SetSlotTask(Unk_ov004_022bf158* state, Task* task, int slot);
void Ov004_StartClock(Unk_ov004_022bf158* state);
void Ov004_Init3d(void);
void Ov004_SendTexturedQuad(int x, int y, int z, int sSize, int tSize, int w,
                            int h, int s0, int t0, u32 texAddr, u32 palAddr);
int func_ov004_022ada10(void);
void func_ov004_022bb1d8(Unk_ov004_022bf158* state);
void Ov004_UnpackResource(const char* name, void* dest);
void Ov004_CopyResource(const char* name, void* dest);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);
void Task_ResumeScript(void* task);
Task* PaletteFade_StartFromBlack(int a, void* b, int c, int d, int e);
Task* PaletteFade_StartToBlack(int a, int b, int c, int d);

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern u32 data_ov004_022bdc94[]; /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84
    []; /* map bytes of a rotation/scaling BG by screen size */

extern char data_ov004_022bdd60[];
extern char data_ov004_022bdd64[];
extern char data_ov004_022bdd84[];
extern char data_ov004_022bdd8c[];
extern char data_ov004_022bdd90[];
extern char data_ov004_022bdd98[];
extern char data_ov004_022bddb8[];
extern char data_ov004_022bdde0[];
extern char data_ov004_022bdde4[];
extern char data_ov004_022bdde8[];
extern char data_ov004_022bddf0[];
extern char data_ov004_022bddf4[];
extern char data_ov004_022bde08[];

extern TaskScriptCmd data_ov004_022be478[];
extern TaskScriptCmd data_ov004_022be4b8[];
extern TaskScriptCmd data_ov004_022be4d8[];

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

/* Puts newTask in the next free slot. In this script the task's duration
 * field counts the slots used so far. */
#define ADD_SLOT_TASK(newTask)               \
  do {                                       \
    int slot = task->duration++;             \
    Ov004_SetSlotTask(state, newTask, slot); \
  } while (0)

/* The screen that shows the 3D engine (BG0 in mode 1, Ov004Task_InitScene). */
#define SCREEN_3D (data_027c0288.unk_00)

/* Makes SCREEN_3D current; RESTORE_SCREEN puts the previous screen back. */
#define USE_3D_SCREEN()                   \
  Unk_027c0280* savedGfx = data_027c0280; \
  Unk_02080440* savedState;               \
  Screen_SetCurrentGfx(SCREEN_3D);        \
  savedState = data_027c027c;             \
  Screen_SetCurrent(SCREEN_3D)

#define RESTORE_SCREEN()         \
  Screen_SetCurrent(savedState); \
  Screen_SetCurrentGfx(savedGfx)

#define SHOW_LAYERS(b0, b1, b2, b3, o)    \
  GFX_DISPCNT(data_027c0280)->bg0 = (b0); \
  GFX_DISPCNT(data_027c0280)->bg1 = (b1); \
  GFX_DISPCNT(data_027c0280)->bg2 = (b2); \
  GFX_DISPCNT(data_027c0280)->bg3 = (b3); \
  GFX_DISPCNT(data_027c0280)->obj = (o)

/* Loads tiles, map and palette into BG layer i and adds addend to every map entry. */
#define LOAD_LAYER(i, bg, tiles, tileMap, palette, palDest, addend)  \
  do {                                                               \
    u16* entries;                                                    \
    Ov004_UnpackResource(tiles, BG_CHAR_DATA(bg));                   \
    Ov004_UnpackResource(tileMap, GFX_LAYER(data_027c0280, i)->map); \
    Ov004_CopyResource(palette, palDest);                            \
    entries = GFX_LAYER(data_027c0280, i)->map;                      \
    TileMap_AddToEntries(entries, LAYER_MAP_BYTES(i), addend);       \
    Gfx_RequestLayerCopy(i);                                         \
    Palette_Flush();                                                 \
  } while (0)

/* Script step that plays a timeline of cues on the 3D screen. It shows a
 * scaled BG3 picture at frame 106, then three times loads a picture into BG2,
 * shows it, and switches to a 256x128 textured quad on BG0. At frame 156 it
 * loads BG1 and shows it, at frame 170 it shows BG3 at normal scale, and at
 * frame 223 it resumes the script. In this step task->tweenFrames picks the
 * quad drawn each frame: 0 none, 1 the texture at 0x8000, 2 the one at 0xc000. */
void Ov004Task_RunPictureQuadTimeline(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  Ov004_UpdateElapsedFrames(state);

  if (Ov004_CheckTimedEvent(state, 0, 0)) {
    ADD_SLOT_TASK(
        PaletteFade_StartFromBlack(5, &state->savedPalette[16 * 1], 30, 1, 1));
    ADD_SLOT_TASK(Task_Create(data_ov004_022be478, TASK_LIST(5)));
  }
  if (Ov004_CheckTimedEvent(state, 1, 82)) {
    ADD_SLOT_TASK(PaletteFade_StartToBlack(5, 24, 1, 1));
  }
  if (Ov004_CheckTimedEvent(state, 2, 106)) {
    {
      USE_3D_SCREEN();
      data_027c0280->masterBrightness = 16;
      RESTORE_SCREEN();
    }
    ADD_SLOT_TASK(func_020fcd28(TASK_LIST(5), 13));
    {
      USE_3D_SCREEN();
      SHOW_LAYERS(0, 0, 0, 1, 0);
      RESTORE_SCREEN();
    }
    Ov004_SetBg3Params(state, 128, 96, 128, 96, 336, 336, 0);
    ADD_SLOT_TASK(Task_Create(data_ov004_022be4b8, TASK_LIST(5)));
  }
  if (Ov004_CheckTimedEvent(state, 3, 120)) {
    USE_3D_SCREEN();
    LOAD_LAYER(2, data_027c0280->layer2Bg, data_ov004_022bdd84,
               data_ov004_022bddf0, data_ov004_022bdd98,
               data_027c0280->paletteBuffer + 0x20, 0x1000);
    RESTORE_SCREEN();
  }
  if (Ov004_CheckTimedEvent(state, 4, 122)) {
    USE_3D_SCREEN();
    SHOW_LAYERS(0, 0, 1, 0, 0);
    RESTORE_SCREEN();
  }
  if (Ov004_CheckTimedEvent(state, 5, 130)) {
    USE_3D_SCREEN();
    SHOW_LAYERS(1, 0, 0, 0, 0);
    task->tweenFrames = 1;
    RESTORE_SCREEN();
  }
  if (Ov004_CheckTimedEvent(state, 6, 134)) {
    USE_3D_SCREEN();
    LOAD_LAYER(2, data_027c0280->layer2Bg, data_ov004_022bdde0,
               data_ov004_022bddf4, data_ov004_022bdd64,
               data_027c0280->paletteBuffer + 0x20, 0x1000);
    RESTORE_SCREEN();
  }
  if (Ov004_CheckTimedEvent(state, 7, 136)) {
    USE_3D_SCREEN();
    SHOW_LAYERS(0, 0, 1, 0, 0);
    RESTORE_SCREEN();
  }
  if (Ov004_CheckTimedEvent(state, 8, 142)) {
    USE_3D_SCREEN();
    SHOW_LAYERS(1, 0, 0, 0, 0);
    task->tweenFrames = 2;
    RESTORE_SCREEN();
  }
  if (Ov004_CheckTimedEvent(state, 9, 148)) {
    USE_3D_SCREEN();
    LOAD_LAYER(2, data_027c0280->layer2Bg, data_ov004_022bdde4,
               data_ov004_022bdd90, data_ov004_022bdd60,
               data_027c0280->paletteBuffer, 0);
    RESTORE_SCREEN();
  }
  if (Ov004_CheckTimedEvent(state, 10, 150)) {
    USE_3D_SCREEN();
    SHOW_LAYERS(0, 0, 1, 0, 0);
    task->tweenFrames = 0;
    RESTORE_SCREEN();
  }
  if (Ov004_CheckTimedEvent(state, 11, 156)) {
    USE_3D_SCREEN();
    LOAD_LAYER(1, data_027c0280->layer1Bg, data_ov004_022bdde8,
               data_ov004_022bdd8c, data_ov004_022bde08,
               data_027c0280->paletteBuffer + 0x100, 0x8000);
    RESTORE_SCREEN();
  }
  if (Ov004_CheckTimedEvent(state, 12, 158)) {
    USE_3D_SCREEN();
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    SHOW_LAYERS(0, 1, 0, 0, 0);
    Ov004_CopyResource(data_ov004_022bddb8, data_027c0280->paletteBuffer);
    Palette_Flush();
    RESTORE_SCREEN();
  }
  if (Ov004_CheckTimedEvent(state, 13, 170)) {
    {
      USE_3D_SCREEN();
      SHOW_LAYERS(0, 0, 0, 1, 0);
      RESTORE_SCREEN();
    }
    Ov004_SetBg3Params(state, 128, 96, 128, 96, 256, 256, 0);
    ADD_SLOT_TASK(Task_Create(data_ov004_022be4d8, TASK_LIST(5)));
  }
  if (Ov004_CheckTimedEvent(state, 14, 200)) {
    ADD_SLOT_TASK(func_020fccd8(TASK_LIST(5), 23));
  }
  if (Ov004_CheckTimedEvent(state, 15, 223)) {
    Ov004_StartClock(state);
    func_ov004_022bb1d8(state);
    Task_ResumeScript(task);
    return;
  }

  if (task->tweenFrames == 0) {
    Ov004_Init3d();
    func_ov004_022ada10();
  }
  if (task->tweenFrames == 1) {
    Ov004_Init3d();
    Ov004_SendTexturedQuad(-104, -90, 0, 5, 4, 256, 128, 0, 0, 0x8000, 160);
    func_ov004_022ada10();
  }
  if (task->tweenFrames == 2) {
    Ov004_Init3d();
    Ov004_SendTexturedQuad(-152, 0, 0, 5, 4, 256, 128, 0, 0, 0xc000, 160);
    func_ov004_022ada10();
  }
}

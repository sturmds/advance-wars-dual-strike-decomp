#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern u32 data_ov004_022bdc94[]; /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84
    []; /* map bytes of a rotation/scaling BG by screen size */

extern char data_ov004_022bdd94[]; /* "095": layer 3 map */
extern char data_ov004_022bddd4[]; /* "092": layer 3 tiles */
extern char data_ov004_022bdddc[]; /* "091": layer 1 tiles */
extern char data_ov004_022bddfc[]; /* "094": layer 1 map */
extern char data_ov004_022bde10[]; /* "093": palette for BG palette 8 */

extern TaskScriptCmd data_ov004_022bdff8[];
extern TaskScriptCmd data_ov004_022be058[];
extern TaskScriptCmd data_ov004_022be318[];
extern TaskScriptCmd data_ov004_022be378[];

void Task_ResumeScript(void* task);
void Ov004_Init3d(void);
void Ov004_SendTexturedQuad(int x, int y, int z, int sSize, int tSize, int w,
                            int h, int s0, int t0, u32 texAddr, u32 palAddr);
int func_ov004_022ada10(void);
void func_ov004_022bb1d8(Unk_ov004_022bf158* state);
void Ov004_SetSlotTask(Unk_ov004_022bf158* state, void* task, int id);
void Ov004_DestroySlotTasks(Unk_ov004_022bf158* state);
void Ov004_UpdateElapsedFrames(Unk_ov004_022bf158* state);
int Ov004_CheckTimedEvent(Unk_ov004_022bf158* state, int event, int frame);
void Ov004_StartClock(Unk_ov004_022bf158* state);
void Ov004_UnpackResource(const char* name, void* dst);
void Ov004_CopyResource(const char* name, void* dst);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
void Gfx_ClearLayerMap(int layer);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);

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

#define START_SLOT_TASK(script)                                      \
  do {                                                               \
    int id = task->duration++;                                       \
    Ov004_SetSlotTask(state, Task_Create(script, TASK_LIST(5)), id); \
  } while (0)

/* The screen that shows the 3D engine (BG0 in mode 1). */
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

/* Loads tiles and map into BG layer i, the palette into BG palette 8, and
 * points every map entry at that palette. Needed to match: `entries` in its
 * own block, like the saved screens in USE_3D_SCREEN. At function scope they
 * swap the registers of task and state. */
#define LOAD_LAYER(i, bg, tiles, tileMap)                          \
  Ov004_UnpackResource(tiles, BG_CHAR_DATA(bg));                   \
  Ov004_UnpackResource(tileMap, GFX_LAYER(data_027c0280, i)->map); \
  Ov004_CopyResource(data_ov004_022bde10,                          \
                     data_027c0280->paletteBuffer + 0x100);        \
  {                                                                \
    u16* entries = GFX_LAYER(data_027c0280, i)->map;               \
    TileMap_AddToEntries(entries, LAYER_MAP_BYTES(i), 0x8000);     \
  }                                                                \
  Gfx_RequestLayerCopy(i);                                         \
  Palette_Flush()

/* Runs a timeline of cues (Ov004_CheckTimedEvent, each fires once): frame 0
 * shows BG0, BG2 and OBJ and starts two script tasks, frame 18 loads pictures
 * into layers 1 and 3, frame 20 shows every layer, blends layers 1 and 3 over
 * layer 2 and starts two more tasks, frame 219 blacks the screen out and
 * resumes the script. Until then it draws two 256x128 textured quads that
 * slide with state->unk_8f8. task->duration counts the task slots handed out. */
void Ov004Task_RunSlidingQuadsTimeline(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  Ov004_UpdateElapsedFrames(state);

  if (Ov004_CheckTimedEvent(state, 0, 0)) {
    USE_3D_SCREEN();
    SHOW_LAYERS(1, 0, 1, 0, 1);
    data_027c0280->masterBrightness = 0;
    RESTORE_SCREEN();
    state->unk_8f8 = -144;
    START_SLOT_TASK(data_ov004_022bdff8);
    START_SLOT_TASK(data_ov004_022be058);
  }
  if (Ov004_CheckTimedEvent(state, 1, 18)) {
    Gfx_ClearLayerMap(1);
    Gfx_ClearLayerMap(3);
    LOAD_LAYER(1, data_027c0280->layer1Bg, data_ov004_022bdddc,
               data_ov004_022bddfc);
    LOAD_LAYER(3, data_027c0280->layer3Bg, data_ov004_022bddd4,
               data_ov004_022bdd94);
  }
  if (Ov004_CheckTimedEvent(state, 2, 20)) {
    USE_3D_SCREEN();
    SHOW_LAYERS(1, 1, 1, 1, 1);
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 192;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = -64;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 0;
    data_027c0280->blendEvb = 16;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBackdrop = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBackdrop = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendBottomObj = 0;
    RESTORE_SCREEN();
    START_SLOT_TASK(data_ov004_022be318);
    START_SLOT_TASK(data_ov004_022be378);
  }
  if (Ov004_CheckTimedEvent(state, 3, 219)) {
    Ov004_StartClock(state);
    {
      USE_3D_SCREEN();
      data_027c0280->masterBrightness = -16;
      RESTORE_SCREEN();
    }
    Ov004_DestroySlotTasks(state);
    func_ov004_022bb1d8(state);
    Task_ResumeScript(task);
  } else {
    Ov004_Init3d();
    Ov004_SendTexturedQuad(state->unk_8f8 + 16, 16, 0, 5, 4, 256, 128, 0, 0,
                           0x8000, 160);
    Ov004_SendTexturedQuad(state->unk_8f8 + 272, 16, 0, 5, 4, 256, 128, 0, 0,
                           0xc000, 160);
    func_ov004_022ada10();
  }
}

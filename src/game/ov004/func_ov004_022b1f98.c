#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158 *data_ov004_022bf158;
extern u32 data_ov004_022bdc94[];       /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84[];       /* map bytes of a rotation/scaling BG by screen size */

extern char data_ov004_022bdd94[];      /* "095": layer 3 map */
extern char data_ov004_022bddd4[];      /* "092": layer 3 tiles */
extern char data_ov004_022bdddc[];      /* "091": layer 1 tiles */
extern char data_ov004_022bddfc[];      /* "094": layer 1 map */
extern char data_ov004_022bde10[];      /* "093": palette for BG palette 8 */

extern TaskScriptCmd data_ov004_022bdff8[];
extern TaskScriptCmd data_ov004_022be058[];
extern TaskScriptCmd data_ov004_022be318[];
extern TaskScriptCmd data_ov004_022be378[];

void Task_ResumeScript(void *task);
void Ov004_Init3d(void);
void Ov004_SendTexturedQuad(int x, int y, int z, int sSize, int tSize, int w, int h, int s0, int t0, u32 texAddr, u32 palAddr);
int func_ov004_022ada10(void);
void func_ov004_022bb1d8(Unk_ov004_022bf158 *state);
void Ov004_SetSlotTask(Unk_ov004_022bf158 *state, void *task, int id);
void Ov004_DestroySlotTasks(Unk_ov004_022bf158 *state);
void Ov004_UpdateElapsedFrames(Unk_ov004_022bf158 *state);
int Ov004_CheckTimedEvent(Unk_ov004_022bf158 *state, int event, int frame);
void Ov004_StartClock(Unk_ov004_022bf158 *state);
void Ov004_UnpackResource(const char *name, void *dst);
void Ov004_CopyResource(const char *name, void *dst);
void TileMap_AddToEntries(u16 *entries, u16 byteCount, int addend);
void Gfx_ClearLayerMap(int layer);
BOOL func_01ff8ef8(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);

/* Byte size of layer i's tile map. */
#define LAYER_MAP_BYTES(i)                                                                  \
    (func_01ff8ef8(i) ? data_ov004_022bdc94[GFX_LAYER(data_027c0280, i)->screenSize]        \
     : Screen_IsAffineBg(i) ? data_ov004_022bdc84[GFX_LAYER(data_027c0280, i)->screenSize]  \
                            : data_ov004_022bdc84[GFX_LAYER(data_027c0280, i)->screenSize] * 2)

/* Tile data of BG bg (BG VRAM plus its character base). */
#define BG_CHAR_DATA(bg) (data_027c0280->bgVram + (data_027c0280->bgCnt[bg].charBase << 14))

#define START_SLOT_TASK(script)                                                             \
    do {                                                                                    \
        int id = task->duration++;                                                          \
        Ov004_SetSlotTask(state, Task_Create(script, TASK_LIST(5)), id);                    \
    } while (0)

/* Runs a timeline of cues (Ov004_CheckTimedEvent, each fires once): frame 0
 * shows BG0, BG2 and OBJ and starts two script tasks, frame 18 loads pictures
 * into layers 1 and 3, frame 20 shows every layer, blends layers 1 and 3 over
 * layer 2 and starts two more tasks, frame 219 blacks the screen out and
 * resumes the script. Until then it draws two 256x128 textured quads that
 * slide with state->unk_8f8. task->duration counts the task slots handed out.
 * WIP: task and state land in swapped registers (r5/r4 instead of r4/r5). */
void func_ov004_022b1f98(Ov004Task *task)
{
    Unk_ov004_022bf158 *state = data_ov004_022bf158;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;
    u16 *map;

    Ov004_UpdateElapsedFrames(state);

    if (Ov004_CheckTimedEvent(state, 0, 0)) {
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        GFX_DISPCNT(data_027c0280)->bg0 = 1;
        GFX_DISPCNT(data_027c0280)->bg1 = 0;
        GFX_DISPCNT(data_027c0280)->bg2 = 1;
        GFX_DISPCNT(data_027c0280)->bg3 = 0;
        GFX_DISPCNT(data_027c0280)->obj = 1;
        data_027c0280->masterBrightness = 0;
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        state->unk_8f8 = -144;
        START_SLOT_TASK(data_ov004_022bdff8);
        START_SLOT_TASK(data_ov004_022be058);
    }
    if (Ov004_CheckTimedEvent(state, 1, 18)) {
        Gfx_ClearLayerMap(1);
        Gfx_ClearLayerMap(3);

        Ov004_UnpackResource(data_ov004_022bdddc, BG_CHAR_DATA(data_027c0280->layer1Bg));
        Ov004_UnpackResource(data_ov004_022bddfc, GFX_LAYER(data_027c0280, 1)->map);
        Ov004_CopyResource(data_ov004_022bde10, data_027c0280->paletteBuffer + 0x100);
        map = GFX_LAYER(data_027c0280, 1)->map;
        TileMap_AddToEntries(map, LAYER_MAP_BYTES(1), 0x8000);
        Gfx_RequestLayerCopy(1);
        Palette_Flush();

        Ov004_UnpackResource(data_ov004_022bddd4, BG_CHAR_DATA(data_027c0280->layer3Bg));
        Ov004_UnpackResource(data_ov004_022bdd94, GFX_LAYER(data_027c0280, 3)->map);
        Ov004_CopyResource(data_ov004_022bde10, data_027c0280->paletteBuffer + 0x100);
        map = GFX_LAYER(data_027c0280, 3)->map;
        TileMap_AddToEntries(map, LAYER_MAP_BYTES(3), 0x8000);
        Gfx_RequestLayerCopy(3);
        Palette_Flush();
    }
    if (Ov004_CheckTimedEvent(state, 2, 20)) {
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        GFX_DISPCNT(data_027c0280)->bg0 = 1;
        GFX_DISPCNT(data_027c0280)->bg1 = 1;
        GFX_DISPCNT(data_027c0280)->bg2 = 1;
        GFX_DISPCNT(data_027c0280)->bg3 = 1;
        GFX_DISPCNT(data_027c0280)->obj = 1;
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
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        START_SLOT_TASK(data_ov004_022be318);
        START_SLOT_TASK(data_ov004_022be378);
    }
    if (Ov004_CheckTimedEvent(state, 3, 219)) {
        Ov004_StartClock(state);
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        data_027c0280->masterBrightness = -16;
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        Ov004_DestroySlotTasks(state);
        func_ov004_022bb1d8(state);
        Task_ResumeScript(task);
    } else {
        Ov004_Init3d();
        Ov004_SendTexturedQuad(state->unk_8f8 + 16, 16, 0, 5, 4, 256, 128, 0, 0, 0x8000, 160);
        Ov004_SendTexturedQuad(state->unk_8f8 + 272, 16, 0, 5, 4, 256, 128, 0, 0, 0xc000, 160);
        func_ov004_022ada10();
    }
}

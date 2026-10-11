#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"

#define REG_MTX_PUSH ((volatile u32 *)0x04000444)
#define REG_MTX_POP ((volatile u32 *)0x04000448)
#define REG_MTX_SCALE ((volatile u32 *)0x0400046c)

extern Unk_ov004_022bf158 *data_ov004_022bf158;
extern u32 gTaskSystemReady;
extern u32 data_ov004_022bdc94[];       /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84[];       /* map bytes of a rotation/scaling BG by screen size */

extern char data_ov004_022bdd68[];
extern char data_ov004_022bdd94[];
extern char data_ov004_022bdda4[];
extern char data_ov004_022bdda8[];
extern char data_ov004_022bddac[];
extern char data_ov004_022bddd4[];
extern char data_ov004_022bdddc[];
extern char data_ov004_022bddfc[];
extern char data_ov004_022bde10[];
extern char data_ov004_022bde14[];
extern char data_ov004_022bde18[];
extern char data_ov004_022bde1c[];
extern char data_ov004_022bde20[];
extern char data_ov004_022bde54[];
extern char data_ov004_022bde64[];
extern char data_ov004_022bde74[];
extern char data_ov004_022bde84[];
extern char data_ov004_022bde94[];
extern char data_ov004_022bdea4[];
extern char data_ov004_022bdeb4[];
extern char data_ov004_022bdec4[];

extern TaskScriptCmd data_ov004_022be018[];
extern TaskScriptCmd data_ov004_022be078[];
extern TaskScriptCmd data_ov004_022be0b8[];
extern TaskScriptCmd data_ov004_022be0d8[];
extern TaskScriptCmd data_ov004_022be138[];
extern TaskScriptCmd data_ov004_022be198[];
extern TaskScriptCmd data_ov004_022be1b8[];
extern TaskScriptCmd data_ov004_022be218[];
extern TaskScriptCmd data_ov004_022be278[];
extern TaskScriptCmd data_ov004_022be298[];
extern TaskScriptCmd data_ov004_022be2f8[];
extern TaskScriptCmd data_ov004_022be338[];
extern TaskScriptCmd data_ov004_022be358[];
extern TaskScriptCmd data_ov004_022be398[];
extern TaskScriptCmd data_ov004_022be3f8[];
extern TaskScriptCmd data_ov004_022be438[];
extern TaskScriptCmd data_ov004_022be558[];
extern TaskScriptCmd data_ov004_022be638[];
extern TaskScriptCmd data_ov004_022be718[];
extern TaskScriptCmd data_ov004_022be7f8[];

void Task_ResumeScript(void *task);
void Ov004_Init3d(void);
void Ov004_SendTexturedQuad(int x, int y, int z, int sSize, int tSize, int w, int h, int s0, int t0, u32 texAddr, u32 palAddr);
void func_ov004_022ad71c(int x, int y, int z, int sSize, int tSize, int w, int h, u32 texAddr, u32 palAddr, int shade);
int func_ov004_022ada10(void);
void func_ov004_022bb1d8(Unk_ov004_022bf158 *state);
void Ov004_ShowLoadedBg3(void);       /* copies the BG3 tiles from Ov004_LoadBg3FromResource to VRAM, shows only BG3 */
void Ov004_SetSlotTask(Unk_ov004_022bf158 *state, void *task, int id);
void Ov004_UpdateElapsedFrames(Unk_ov004_022bf158 *state);
int Ov004_CheckTimedEvent(Unk_ov004_022bf158 *state, int event, int frame);
void Ov004_StartClock(Unk_ov004_022bf158 *state);
void Ov004_UnpackResource(const char *name, void *dst);
void Ov004_CopyResource(const char *name, void *dst);
void Ov004_LoadBg3FromResource(const char *name, int tilesOffset, int mapOffset, int palette);
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

#define SHOW_BG0_TO_BG2()                                                                   \
    do {                                                                                    \
        GFX_DISPCNT(data_027c0280)->bg0 = 1;                                                \
        GFX_DISPCNT(data_027c0280)->bg1 = 1;                                                \
        GFX_DISPCNT(data_027c0280)->bg2 = 1;                                                \
        GFX_DISPCNT(data_027c0280)->bg3 = 0;                                                \
        GFX_DISPCNT(data_027c0280)->obj = 0;                                                \
    } while (0)

/* Runs a timeline of cues (Ov004_CheckTimedEvent, each fires once) on the
 * current screen: it loads BG1 and BG2, then shows eight BG3 pictures in turn
 * from frame 107 to 215, starts script tasks, sets up an alpha blend at frame 330 and
 * resumes the script at frame 438. Until then it draws two 256x128 textured
 * quads and every active entry as a quad scaled by 3 in x and y.
 * task->duration counts the task slots handed out. */
void Ov004Task_RunSceneTimeline(Ov004Task *task)
{
    Unk_ov004_022bf158 *state = data_ov004_022bf158;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;
    u16 *map;
    int i;
    Ov004Entry *entry;

    Ov004_UpdateElapsedFrames(state);
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);

    if (Ov004_CheckTimedEvent(state, 0, 0)) {
        int id;

        state->unk_8f8 = 230;
        state->unk_8fc = -32;
        START_SLOT_TASK(data_ov004_022be078);
        /* list 0 does not run while gTaskSystemReady is 0 */
        gTaskSystemReady = 0;
        id = task->duration++;
        Ov004_SetSlotTask(state, Task_Create(data_ov004_022be3f8, TASK_LIST(0)), id);
        gTaskSystemReady = 1;
        Gfx_ClearLayerMap(1);
        Gfx_ClearLayerMap(2);

        /* BG1 and BG2 use BG palette 8: map entries get 0x8000, colours go to paletteBuffer + 0x100 */
        Ov004_UnpackResource(data_ov004_022bdddc, BG_CHAR_DATA(data_027c0280->layer1Bg));
        Ov004_UnpackResource(data_ov004_022bddfc, GFX_LAYER(data_027c0280, 1)->map);
        Ov004_CopyResource(data_ov004_022bde10, data_027c0280->paletteBuffer + 0x100);
        map = GFX_LAYER(data_027c0280, 1)->map;
        TileMap_AddToEntries(map, LAYER_MAP_BYTES(1), 0x8000);
        Gfx_RequestLayerCopy(1);
        Palette_Flush();

        Ov004_UnpackResource(data_ov004_022bddd4, BG_CHAR_DATA(data_027c0280->layer2Bg));
        Ov004_UnpackResource(data_ov004_022bdd94, GFX_LAYER(data_027c0280, 2)->map);
        Ov004_CopyResource(data_ov004_022bde10, data_027c0280->paletteBuffer + 0x100);
        map = GFX_LAYER(data_027c0280, 2)->map;
        TileMap_AddToEntries(map, LAYER_MAP_BYTES(2), 0x8000);
        Gfx_RequestLayerCopy(2);
        Palette_Flush();
    }
    if (Ov004_CheckTimedEvent(state, 39, 1)) {
        GFX_DISPCNT(data_027c0280)->bg0 = 1;
        GFX_DISPCNT(data_027c0280)->bg1 = 1;
        GFX_DISPCNT(data_027c0280)->bg2 = 1;
        GFX_DISPCNT(data_027c0280)->bg3 = 0;
        GFX_DISPCNT(data_027c0280)->obj = 0;
        data_027c0280->masterBrightness = 0;
        data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 192;
        data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
        data_027c0280->bgScrollY[data_027c0280->layer2Bg] = -64;
        data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    }
    if (Ov004_CheckTimedEvent(state, 1, 107)) {
        Ov004_LoadBg3FromResource(data_ov004_022bde84, 0, 0x94d8, 0);
    }
    if (Ov004_CheckTimedEvent(state, 2, 110)) {
        data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
        Ov004_ShowLoadedBg3();
        START_SLOT_TASK(data_ov004_022be0d8);
    }
    if (Ov004_CheckTimedEvent(state, 3, 127)) {
        Ov004_LoadBg3FromResource(data_ov004_022bde54, 0, 0xa4a0, 1);
    }
    if (Ov004_CheckTimedEvent(state, 4, 130)) {
        data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 64;
        Ov004_ShowLoadedBg3();
        START_SLOT_TASK(data_ov004_022be1b8);
    }
    if (Ov004_CheckTimedEvent(state, 5, 150)) {
        SHOW_BG0_TO_BG2();
        state->bgCharBase = 0;
    }
    if (Ov004_CheckTimedEvent(state, 6, 157)) {
        Ov004_LoadBg3FromResource(data_ov004_022bde74, 0, 0x4990, 2);
    }
    /* Event 0 already fired at frame 0, so this never runs. */
    if (Ov004_CheckTimedEvent(state, 0, 160)) {
        data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
        Ov004_ShowLoadedBg3();
        START_SLOT_TASK(data_ov004_022be298);
    }
    if (Ov004_CheckTimedEvent(state, 7, 167)) {
        Ov004_LoadBg3FromResource(data_ov004_022bdec4, 0, 0x5590, 3);
    }
    if (Ov004_CheckTimedEvent(state, 8, 170)) {
        data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 64;
        Ov004_ShowLoadedBg3();
        START_SLOT_TASK(data_ov004_022be358);
    }
    if (Ov004_CheckTimedEvent(state, 9, 177)) {
        Ov004_LoadBg3FromResource(data_ov004_022bde94, 0, 0x59e0, 4);
    }
    if (Ov004_CheckTimedEvent(state, 10, 180)) {
        data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
        Ov004_ShowLoadedBg3();
        START_SLOT_TASK(data_ov004_022be298);
    }
    if (Ov004_CheckTimedEvent(state, 11, 190)) {
        SHOW_BG0_TO_BG2();
        state->bgCharBase = 0;
    }
    if (Ov004_CheckTimedEvent(state, 12, 192)) {
        Ov004_LoadBg3FromResource(data_ov004_022bde64, 0, 0x4718, 5);
    }
    if (Ov004_CheckTimedEvent(state, 13, 195)) {
        data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
        Ov004_ShowLoadedBg3();
    }
    if (Ov004_CheckTimedEvent(state, 14, 200)) {
        SHOW_BG0_TO_BG2();
        state->bgCharBase = 0;
    }
    if (Ov004_CheckTimedEvent(state, 15, 202)) {
        Ov004_LoadBg3FromResource(data_ov004_022bdea4, 0, 0x3b64, 6);
    }
    if (Ov004_CheckTimedEvent(state, 16, 205)) {
        data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 64;
        Ov004_ShowLoadedBg3();
    }
    if (Ov004_CheckTimedEvent(state, 17, 210)) {
        SHOW_BG0_TO_BG2();
        state->bgCharBase = 0;
    }
    if (Ov004_CheckTimedEvent(state, 18, 212)) {
        Ov004_LoadBg3FromResource(data_ov004_022bdeb4, 0, 0x3e84, 7);
    }
    if (Ov004_CheckTimedEvent(state, 19, 215)) {
        data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
        Ov004_ShowLoadedBg3();
    }
    if (Ov004_CheckTimedEvent(state, 20, 220)) {
        SHOW_BG0_TO_BG2();
        state->bgCharBase = 0;
        START_SLOT_TASK(data_ov004_022be0b8);
    }
    if (Ov004_CheckTimedEvent(state, 21, 227)) {
        START_SLOT_TASK(data_ov004_022be138);
    }
    if (Ov004_CheckTimedEvent(state, 22, 234)) {
        START_SLOT_TASK(data_ov004_022be198);
    }
    if (Ov004_CheckTimedEvent(state, 23, 241)) {
        START_SLOT_TASK(data_ov004_022be218);
    }
    if (Ov004_CheckTimedEvent(state, 24, 248)) {
        START_SLOT_TASK(data_ov004_022be278);
    }
    if (Ov004_CheckTimedEvent(state, 25, 255)) {
        START_SLOT_TASK(data_ov004_022be2f8);
    }
    if (Ov004_CheckTimedEvent(state, 26, 262)) {
        START_SLOT_TASK(data_ov004_022be338);
    }
    if (Ov004_CheckTimedEvent(state, 27, 276)) {
        START_SLOT_TASK(data_ov004_022be438);
    }
    if (Ov004_CheckTimedEvent(state, 28, 290)) {
        START_SLOT_TASK(data_ov004_022be398);
    }
    if (Ov004_CheckTimedEvent(state, 29, 303)) {
        data_027c0280->bgCnt[data_027c0280->layer3Bg].colorMode = 0;
        data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
        Ov004_UnpackResource(data_ov004_022bde18, BG_CHAR_DATA(data_027c0280->layer3Bg));
        Ov004_UnpackResource(data_ov004_022bde1c, GFX_LAYER(data_027c0280, 3)->map);
        map = GFX_LAYER(data_027c0280, 3)->map;
        TileMap_AddToEntries(map, LAYER_MAP_BYTES(3), 0);
        Gfx_RequestLayerCopy(3);
        Ov004_CopyResource(data_ov004_022bdda8, data_027c0280->paletteBuffer);
        Palette_Flush();
    }
    if (Ov004_CheckTimedEvent(state, 30, 305)) {
        START_SLOT_TASK(data_ov004_022be558);
    }
    if (Ov004_CheckTimedEvent(state, 31, 330)) {
        data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 3;
        data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
        data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
        data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 0;
        GFX_DISPCNT(data_027c0280)->bg0 = 0;
        GFX_DISPCNT(data_027c0280)->bg1 = 1;
        GFX_DISPCNT(data_027c0280)->bg2 = 1;
        GFX_DISPCNT(data_027c0280)->bg3 = 1;
        GFX_DISPCNT(data_027c0280)->obj = 0;
        data_027c0280->blendMode = BLEND_MODE_ALPHA;
        data_027c0280->blendEva = 0;
        data_027c0280->blendEvb = 16;
        data_027c0280->blendEvy = 0;
        data_027c0280->blendTopBackdrop = 0;
        data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
        data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
        data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
        data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
        data_027c0280->blendTopObj = 0;
        data_027c0280->blendBottomBackdrop = 0;
        data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
        data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
        data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
        data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
        data_027c0280->blendBottomObj = 0;
        START_SLOT_TASK(data_ov004_022be718);
        START_SLOT_TASK(data_ov004_022be018);
    }
    if (Ov004_CheckTimedEvent(state, 32, 407)) {
        Ov004_UnpackResource(data_ov004_022bde14, BG_CHAR_DATA(data_027c0280->layer1Bg));
        Ov004_UnpackResource(data_ov004_022bdda4, GFX_LAYER(data_027c0280, 1)->map);
        map = GFX_LAYER(data_027c0280, 1)->map;
        TileMap_AddToEntries(map, LAYER_MAP_BYTES(1), 0x8000);
        Gfx_RequestLayerCopy(1);
        Ov004_UnpackResource(data_ov004_022bde20, BG_CHAR_DATA(data_027c0280->layer2Bg));
        Ov004_UnpackResource(data_ov004_022bdd68, GFX_LAYER(data_027c0280, 2)->map);
        map = GFX_LAYER(data_027c0280, 2)->map;
        TileMap_AddToEntries(map, LAYER_MAP_BYTES(2), 0x8000);
        Gfx_RequestLayerCopy(2);
        Ov004_CopyResource(data_ov004_022bddac, data_027c0280->paletteBuffer + 0x100);
        Palette_Flush();
        START_SLOT_TASK(data_ov004_022be638);
        START_SLOT_TASK(data_ov004_022be7f8);
    }
    if (Ov004_CheckTimedEvent(state, 33, 438)) {
        Ov004_StartClock(state);
        func_ov004_022bb1d8(state);
        Task_ResumeScript(task);
        return;
    }

    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    Ov004_Init3d();
    Ov004_SendTexturedQuad(-256, state->unk_8fc, state->unk_8f8, 5, 4, 256, 128, 0, 0, 0x8000, 160);
    Ov004_SendTexturedQuad(0, state->unk_8fc, state->unk_8f8, 5, 4, 256, 128, 0, 0, 0xc000, 160);
    for (i = 0, entry = state->entries; i < 7; entry++, i++) {
        if (entry->active) {
            *REG_MTX_PUSH = 0;
            *REG_MTX_SCALE = 0x3000;
            *REG_MTX_SCALE = 0x3000;
            *REG_MTX_SCALE = 0x1000;
            func_ov004_022ad71c(entry->x, entry->y + state->unk_900, 300, 3, 4,
                                entry->width, entry->height, (entry->texSlot << 12) + 0x10000, 192, entry->shade);
            *REG_MTX_POP = 1;
        }
    }
    func_ov004_022ada10();
}

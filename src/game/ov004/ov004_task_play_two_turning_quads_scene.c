#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/brightness_fade.h"

#define GXFIFO_MTX_PUSH ((volatile u32 *)0x04000444)
#define GXFIFO_MTX_POP ((volatile u32 *)0x04000448)
#define GXFIFO_MTX_TRANS ((volatile u32 *)0x04000470)

void Ov004_UpdateElapsedFrames(Unk_ov004_022bf158 *state);
int Ov004_CheckTimedEvent(Unk_ov004_022bf158 *state, int event, int frame);
void Ov004_SetSlotTask(Unk_ov004_022bf158 *state, Task *task, int slot);
void Ov004_StartClock(Unk_ov004_022bf158 *state);
void Ov004_Init3d(void);
void Ov004_SendTexturedQuad(int x, int y, int z, int sSize, int tSize, int w, int h, int s0, int t0, u32 texAddr, u32 palAddr);
int func_ov004_022ada10(void);
void func_ov004_022bb1d8(Unk_ov004_022bf158 *state);
/* name: a resource name, or a resource id below 0x10000 cast to a pointer. */
void Ov004_CopyResource(const char *name, void *dest);
void Ov004_UnpackResource(const char *name, void *dest);
void Ov004_BlitBlock(Unk_ov004_022bf158 *state, int col, int row, const char *name);
void Ov004_BlitBlockMirrored(Unk_ov004_022bf158 *state, int col, int row, const char *name);
void Ov004_FillBlock(Unk_ov004_022bf158 *state, int col, int row, int fill);
void Ov004_InitTileMap(Unk_ov004_022bf158 *state, int cols, int rows);
void func_0210bb60(int sin, int cos);
void func_0200270c(int x, int y, int attr, int list, int hFlip);
void func_01ff8a00(u16 *entries, u16 byteCount, int addend);
void Bg_FillRect(u16 *map, int x, int y, int width, int height, int tile);
BOOL func_01ff8ef8(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);
void Task_ResumeScript(void *task);

extern Unk_ov004_022bf158 *data_ov004_022bf158;
extern s16 data_0214d4c4[];
extern u32 data_ov004_022bdc94[];       /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84[];       /* map bytes of a rotation/scaling BG by screen size */

extern char data_ov004_022bdd48[];
extern char data_ov004_022bdd74[];
extern char data_ov004_022bdd78[];
extern char data_ov004_022bdd7c[];
extern char data_ov004_022bddb8[];
extern char data_ov004_022bddbc[];
extern char data_ov004_022bddc0[];
extern char data_ov004_022bddc4[];
extern char data_ov004_022bddc8[];

extern TaskScriptCmd data_ov004_022bdf98[];
extern TaskScriptCmd data_ov004_022be518[];
extern TaskScriptCmd data_ov004_022be578[];
extern TaskScriptCmd data_ov004_022be598[];
extern TaskScriptCmd data_ov004_022be5d8[];
extern TaskScriptCmd data_ov004_022be5f8[];
extern TaskScriptCmd data_ov004_022be678[];
extern TaskScriptCmd data_ov004_022be6b8[];
extern TaskScriptCmd data_ov004_022be6d8[];
extern TaskScriptCmd data_ov004_022be738[];
extern TaskScriptCmd data_ov004_022be758[];
extern TaskScriptCmd data_ov004_022be798[];
extern TaskScriptCmd data_ov004_022be7b8[];

/* The screen that shows the 3D engine (BG0 in mode 1, Ov004Task_InitScene). */
#define SCREEN_3D (data_027c0288.unk_00)

/* Puts newTask in the next free slot. In this script the task's duration
 * field counts the slots used so far. */
#define ADD_SLOT_TASK(newTask)                          \
    do {                                                \
        int slot = task->duration++;                    \
        Ov004_SetSlotTask(state, newTask, slot);        \
    } while (0)

/* Byte size of layer i's tile map. */
#define LAYER_MAP_BYTES(i)                                                                  \
    (func_01ff8ef8(i) ? data_ov004_022bdc94[GFX_LAYER(data_027c0280, i)->screenSize]        \
     : Screen_IsAffineBg(i) ? data_ov004_022bdc84[GFX_LAYER(data_027c0280, i)->screenSize]  \
                            : data_ov004_022bdc84[GFX_LAYER(data_027c0280, i)->screenSize] * 2)

/* Tile data of BG bg (BG VRAM plus its character base). */
#define BG_CHAR_DATA(bg) (data_027c0280->bgVram + (data_027c0280->bgCnt[bg].charBase << 14))

#define SET_LAYERS_SHOWN(b0, b1, b2, b3, o)                 \
    do {                                                    \
        GFX_DISPCNT(data_027c0280)->bg0 = (b0);             \
        GFX_DISPCNT(data_027c0280)->bg1 = (b1);             \
        GFX_DISPCNT(data_027c0280)->bg2 = (b2);             \
        GFX_DISPCNT(data_027c0280)->bg3 = (b3);             \
        GFX_DISPCNT(data_027c0280)->obj = (o);              \
    } while (0)

/* Shows only BG0 at full brightness. */
#define SHOW_BG0_ONLY()                                     \
    do {                                                    \
        data_027c0280->masterBrightness = 0;                \
        SET_LAYERS_SHOWN(1, 0, 0, 0, 0);                    \
    } while (0)

/* Loads the same tiles and map into BG1 (palette 1, x scrolled by 256) and
 * BG2 (palette 2), then clears the second 32 by 32 block of BG1's map and
 * the first block of BG2's map. */
#define LOAD_LAYERS_1_AND_2()                                                                   \
    do {                                                                                        \
        u16 *map;                                                                               \
        data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;                             \
        data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;                             \
        data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;                             \
        data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;                             \
        SET_LAYERS_SHOWN(1, 1, 1, 0, 0);                                                        \
        data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 256;                                \
        data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;                                  \
        data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;                                  \
        data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;                                  \
        Ov004_UnpackResource(data_ov004_022bddc0, BG_CHAR_DATA(data_027c0280->layer1Bg));       \
        Ov004_UnpackResource(data_ov004_022bddbc, GFX_LAYER(data_027c0280, 1)->map);            \
        map = GFX_LAYER(data_027c0280, 1)->map;                                                 \
        func_01ff8a00(map, LAYER_MAP_BYTES(1), 0x1000);                                         \
        Bg_FillRect(GFX_LAYER(data_027c0280, 1)->map + 0x400, 0, 0, 32, 32, 0);               \
        Gfx_RequestLayerCopy(1);                                                                \
        Ov004_CopyResource(data_ov004_022bddb8, data_027c0280->paletteBuffer + 0x20);           \
        Ov004_UnpackResource(data_ov004_022bddc0, BG_CHAR_DATA(data_027c0280->layer2Bg));       \
        Ov004_UnpackResource(data_ov004_022bddbc, GFX_LAYER(data_027c0280, 2)->map);            \
        map = GFX_LAYER(data_027c0280, 2)->map;                                                 \
        func_01ff8a00(map, LAYER_MAP_BYTES(2), 0x2000);                                         \
        Bg_FillRect(GFX_LAYER(data_027c0280, 2)->map, 0, 0, 32, 32, 0);                       \
        Gfx_RequestLayerCopy(2);                                                                \
        Ov004_CopyResource(data_ov004_022bddb8, data_027c0280->paletteBuffer + 0x40);           \
        Palette_Flush();                                                                        \
    } while (0)

/* Loads BG1's tiles and palette 1 for a 3 by 3 grid of blocks. */
#define LOAD_GRID_LAYER()                                                                       \
    do {                                                                                        \
        SET_LAYERS_SHOWN(1, 1, 0, 1, 1);                                                        \
        data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0xff00;                             \
        Ov004_UnpackResource(data_ov004_022bdd74, BG_CHAR_DATA(data_027c0280->layer1Bg));       \
        Ov004_CopyResource(data_ov004_022bdd48, data_027c0280->paletteBuffer + 0x20);           \
        Ov004_InitTileMap(state, 3, 3);                                                         \
    } while (0)

/* Script step that plays a timed sequence on the 3D screen, twice over with
 * small changes: it loads the same picture into BG1 and BG2 (frames 25 and
 * 249), then fills BG1 with a 3 by 3 grid of blocks (frame 193, mirrored at
 * frame 417); slot tasks run the moves. At frame 446 it resumes the script.
 * Each frame it draws two 240 by 32 textured quads, each turned by its own
 * angle (unk_900, unk_904) and moved to its own x (unk_8f8, unk_8fc), and
 * two sprites at y 160 (x from unk_908 and unk_90c, the second flipped). */
void Ov004Task_PlayTwoTurningQuadsScene(Ov004Task *task)
{
    Unk_ov004_022bf158 *state = data_ov004_022bf158;
    int idx;
    int quad1X;
    int quad2X;
    int quad2Angle;

    Ov004_UpdateElapsedFrames(state);

    if (Ov004_CheckTimedEvent(state, 0, 0)) {
        Unk_027c0280 *savedGfx = data_027c0280;
        Unk_02080440 *savedState;
        Screen_SetCurrentGfx(SCREEN_3D);
        savedState = data_027c027c;
        Screen_SetCurrent(SCREEN_3D);
        SHOW_BG0_ONLY();
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        ADD_SLOT_TASK(Task_Create(data_ov004_022be518, TASK_LIST(5)));
    }

    if (Ov004_CheckTimedEvent(state, 1, 25)) {
        Unk_027c0280 *savedGfx = data_027c0280;
        Unk_02080440 *savedState;
        Screen_SetCurrentGfx(SCREEN_3D);
        savedState = data_027c027c;
        Screen_SetCurrent(SCREEN_3D);
        LOAD_LAYERS_1_AND_2();
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        ADD_SLOT_TASK(Task_Create(data_ov004_022be598, TASK_LIST(5)));
    }

    if (Ov004_CheckTimedEvent(state, 2, 83)) {
        Unk_027c0280 *savedGfx = data_027c0280;
        Unk_02080440 *savedState;
        Screen_SetCurrentGfx(SCREEN_3D);
        savedState = data_027c027c;
        Screen_SetCurrent(SCREEN_3D);
        SET_LAYERS_SHOWN(1, 0, 0, 1, 1);
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        ADD_SLOT_TASK(Task_Create(data_ov004_022be678, TASK_LIST(5)));
        ADD_SLOT_TASK(Task_Create(data_ov004_022be6d8, TASK_LIST(5)));
        ADD_SLOT_TASK(Task_Create(data_ov004_022be758, TASK_LIST(5)));
        ADD_SLOT_TASK(Task_Create(data_ov004_022be7b8, TASK_LIST(5)));
    }

    if (Ov004_CheckTimedEvent(state, 3, 193)) {
        Unk_027c0280 *savedGfx = data_027c0280;
        Unk_02080440 *savedState;
        Screen_SetCurrentGfx(SCREEN_3D);
        savedState = data_027c027c;
        Screen_SetCurrent(SCREEN_3D);
        LOAD_GRID_LAYER();
        Ov004_FillBlock(state, 0, 0, 0);
        Ov004_FillBlock(state, 1, 0, 0);
        Ov004_FillBlock(state, 2, 0, 0);
        Ov004_BlitBlock(state, 0, 1, data_ov004_022bddc8);
        Ov004_BlitBlock(state, 1, 1, data_ov004_022bddc4);
        Ov004_FillBlock(state, 2, 1, 0);
        Ov004_BlitBlock(state, 0, 2, data_ov004_022bdd78);
        Ov004_BlitBlock(state, 1, 2, data_ov004_022bdd7c);
        Ov004_FillBlock(state, 2, 2, 0);
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        ADD_SLOT_TASK(Task_Create(data_ov004_022be5d8, TASK_LIST(5)));
        ADD_SLOT_TASK(func_020fccd8(TASK_LIST(5), 27));
    }

    if (Ov004_CheckTimedEvent(state, 4, 223)) {
        Ov004_SetBg3Params(state, 96, 96, 352, 96, 256, 256, 0);
        state->unk_8f8 = 256;
        state->unk_900 = 0;
        state->unk_908 = 384;
    }

    if (Ov004_CheckTimedEvent(state, 5, 224)) {
        Unk_027c0280 *savedGfx = data_027c0280;
        Unk_02080440 *savedState;
        Screen_SetCurrentGfx(SCREEN_3D);
        savedState = data_027c027c;
        Screen_SetCurrent(SCREEN_3D);
        SHOW_BG0_ONLY();
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        ADD_SLOT_TASK(Task_Create(data_ov004_022be578, TASK_LIST(5)));
    }

    if (Ov004_CheckTimedEvent(state, 6, 249)) {
        Unk_027c0280 *savedGfx = data_027c0280;
        Unk_02080440 *savedState;
        Screen_SetCurrentGfx(SCREEN_3D);
        savedState = data_027c027c;
        Screen_SetCurrent(SCREEN_3D);
        LOAD_LAYERS_1_AND_2();
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        ADD_SLOT_TASK(Task_Create(data_ov004_022be598, TASK_LIST(5)));
    }

    if (Ov004_CheckTimedEvent(state, 7, 307)) {
        Unk_027c0280 *savedGfx = data_027c0280;
        Unk_02080440 *savedState;
        Screen_SetCurrentGfx(SCREEN_3D);
        savedState = data_027c027c;
        Screen_SetCurrent(SCREEN_3D);
        SET_LAYERS_SHOWN(1, 0, 0, 1, 1);
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        ADD_SLOT_TASK(Task_Create(data_ov004_022be6b8, TASK_LIST(5)));
        ADD_SLOT_TASK(Task_Create(data_ov004_022be738, TASK_LIST(5)));
        ADD_SLOT_TASK(Task_Create(data_ov004_022be798, TASK_LIST(5)));
        ADD_SLOT_TASK(Task_Create(data_ov004_022bdf98, TASK_LIST(5)));
    }

    if (Ov004_CheckTimedEvent(state, 8, 417)) {
        Unk_027c0280 *savedGfx = data_027c0280;
        Unk_02080440 *savedState;
        Screen_SetCurrentGfx(SCREEN_3D);
        savedState = data_027c027c;
        Screen_SetCurrent(SCREEN_3D);
        LOAD_GRID_LAYER();
        Ov004_FillBlock(state, 0, 0, 0);
        Ov004_FillBlock(state, 1, 0, 0);
        Ov004_FillBlock(state, 2, 0, 0);
        Ov004_FillBlock(state, 0, 1, 0);
        Ov004_BlitBlockMirrored(state, 1, 1, data_ov004_022bddc4);
        Ov004_BlitBlockMirrored(state, 2, 1, data_ov004_022bddc8);
        Ov004_FillBlock(state, 0, 2, 0);
        Ov004_BlitBlockMirrored(state, 1, 2, data_ov004_022bdd7c);
        Ov004_BlitBlockMirrored(state, 2, 2, data_ov004_022bdd78);
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        ADD_SLOT_TASK(Task_Create(data_ov004_022be5f8, TASK_LIST(5)));
        ADD_SLOT_TASK(func_020fccd8(TASK_LIST(5), 27));
    }

    if (Ov004_CheckTimedEvent(state, 9, 446)) {
        Ov004_StartClock(state);
        func_ov004_022bb1d8(state);
        Task_ResumeScript(task);
        return;
    }

    Ov004_Init3d();
    /* data_0214d4c4 holds a sine and cosine pair per 16 angle units */
    idx =(state->unk_900 & 0xffff) >> 4;
    quad2Angle = state->unk_904 & 0xffff;
    quad1X = state->unk_8f8;
    quad2X = state->unk_8fc;
    *GXFIFO_MTX_PUSH = 0;
    func_0210bb60(data_0214d4c4[idx * 2], data_0214d4c4[idx * 2 + 1]);
    *GXFIFO_MTX_TRANS = quad1X;
    *GXFIFO_MTX_TRANS = -16;
    *GXFIFO_MTX_TRANS = 0;
    Ov004_SendTexturedQuad(0, 0, 0, 5, 5, 240, 32, 0, 0, 0, 0);
    *GXFIFO_MTX_POP = 1;

    idx = quad2Angle >> 4;
    *GXFIFO_MTX_PUSH = 0;
    func_0210bb60(data_0214d4c4[idx * 2], data_0214d4c4[idx * 2 + 1]);
    *GXFIFO_MTX_TRANS = quad2X;
    *GXFIFO_MTX_TRANS = -16;
    *GXFIFO_MTX_TRANS = 0;
    Ov004_SendTexturedQuad(0, 0, 0, 5, 5, 240, 32, 0, 32, 0, 0);
    *GXFIFO_MTX_POP = 1;
    func_ov004_022ada10();

    {
        Unk_027c0280 *savedGfx = data_027c0280;
        Unk_02080440 *savedState;
        Screen_SetCurrentGfx(SCREEN_3D);
        savedState = data_027c027c;
        Screen_SetCurrent(SCREEN_3D);
        func_0200270c(state->unk_908 & 0x1ff, 160, 0x800, 0, 0);
        func_0200270c(state->unk_90c & 0x1ff, 160, 0x1a00, 0, 1);
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
    }
}

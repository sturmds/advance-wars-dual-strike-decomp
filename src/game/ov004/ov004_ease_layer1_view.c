#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void *task);
void func_ov004_022bacd8(Unk_ov004_022bf158 *state, u16 *map);
void TileMap_AddToEntries(u16 *entries, u16 byteCount, int addend);
BOOL func_01ff8ef8(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);

extern Unk_ov004_022bf158 *data_ov004_022bf158;
extern u32 data_ov004_022bdc94[];       /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84[];       /* map bytes of a rotation/scaling BG by screen size */

/* Size in bytes of the tile map of a layer (as in Ov004_LoadBgLayers1And2). */
#define LAYER_MAP_BYTES(layer, i)                                               \
    (func_01ff8ef8(i) ? data_ov004_022bdc94[(layer)->screenSize]                \
     : Screen_IsAffineBg(i) ? data_ov004_022bdc84[(layer)->screenSize]          \
                            : data_ov004_022bdc84[(layer)->screenSize] * 2)

void Ov004_EaseLayer1View(Ov004Task *task) {
    Unk_ov004_022bf158 *state = data_ov004_022bf158;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedScreen;
    u16 *map;

    switch (task->step) {
    case 0:                            /* falls through into the first frame */
        task->frame = 0;
        task->duration = task->tweenFrames;
        task->step++;
    case 1:
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedScreen = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        state->viewX = Math_Ease(0, task->tweenFrom, task->tweenTo, task->frame, task->duration);
        state->viewY = Math_Ease(0, task->unk_5c, task->unk_60, task->frame, task->duration);
        func_ov004_022bacd8(state, data_027c0280->layer1Map);
        data_027c0280->bgScrollX[data_027c0280->layer1Bg] = state->viewScrollX;
        data_027c0280->bgScrollY[data_027c0280->layer1Bg] = state->viewScrollY;
        map = data_027c0280->layer1Map;
        TileMap_AddToEntries(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 1), 1), 0x1000);
        Gfx_RequestLayerCopy(1);
        Screen_SetCurrent(savedScreen);
        Screen_SetCurrentGfx(savedGfx);
        if (task->frame <= task->duration) {
            task->frame++;
        }
        if (task->frame > task->duration) {
            Task_ResumeScript(task);
        }
        break;
    }
}

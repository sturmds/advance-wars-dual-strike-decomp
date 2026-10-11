#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void *config);
void TileMap_AddToEntries(u16 *entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);
void Mem_Copy32(const void *src, void *dst, u32 size);
void Mem_Zero32(u32 value, void *dst, u32 size);
void Ov004_UnpackResource(const void *name, void *dest);   /* unpacks a named file */
void Ov004_CopyResource(const void *name, void *dest);   /* copies a named file */
void Ov004_LoadObjGraphics(Unk_ov004_022bf158 *state);
void Ov004_CreateSlotTasks(Unk_ov004_022bf158 *state);     /* creates 24 tasks from a table */
void Ov004_SetupOrtho3d(void);
void Ov004_ClearSlots(Unk_ov004_022bf158 *state);

extern Unk_ov004_022bf158 *data_ov004_022bf158;
extern u8 data_ov004_022be908[];        /* main screen layer config */
extern u8 data_ov004_022bdd88[];        /* "08e": layer 2 tiles */
extern u8 data_ov004_022bddf8[];        /* "090": layer 2 map */
extern u8 data_ov004_022bde0c[];        /* "08f": palette */
extern u32 data_ov004_022bdc94[];       /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84[];       /* map bytes of a rotation/scaling BG by screen size */

/* Size in bytes of the tile map of a layer (as in Gfx_LoadBmap083). */
#define LAYER_MAP_BYTES(layer, i)                                               \
    (Screen_IsTextBg(i) ? data_ov004_022bdc94[(layer)->screenSize]                \
     : Screen_IsAffineBg(i) ? data_ov004_022bdc84[(layer)->screenSize]          \
                            : data_ov004_022bdc84[(layer)->screenSize] * 2)

/* Script step: sets up the main screen in mode 0 with BG0, BG2 and OBJ on,
 * priorities 0 to 3, no windows, blending or scroll. Loads files 08e, 090
 * and 08f (tiles, map and palette) into layer 2, keeps the palette in the
 * state and clears the palette buffer (likely for a fade in). Then
 * starts the scene's tasks and switches to the 2D (orthographic) 3D setup. */
void Ov004Task_InitPictureScene(Ov004Task *task)
{
    Unk_ov004_022bf158 *state;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;
    u16 *map;

    state = data_ov004_022bf158;
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Screen_SetBgMode(0);
    Screen_InitLayers(data_ov004_022be908);
    GFX_DISPCNT(data_027c0280)->bg0 = 1;
    GFX_DISPCNT(data_027c0280)->bg1 = 0;
    GFX_DISPCNT(data_027c0280)->bg2 = 1;
    GFX_DISPCNT(data_027c0280)->bg3 = 0;
    GFX_DISPCNT(data_027c0280)->obj = 1;
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    GFX_DISPCNT(data_027c0280)->win0 = 0;
    GFX_DISPCNT(data_027c0280)->win1 = 0;
    GFX_DISPCNT(data_027c0280)->objWin = 0;
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    GFX_DISPCNT(data_027c0280)->objMap1D = 1;
    data_027c0280->unk_dd = 0;
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;

    Ov004_UnpackResource(data_ov004_022bdd88, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
    Ov004_UnpackResource(data_ov004_022bddf8, data_027c0280->layer2Map);
    Ov004_CopyResource(data_ov004_022bde0c, data_027c0280->paletteBuffer);
    map = data_027c0280->layer2Map;
    TileMap_AddToEntries(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 2), 2), 0);
    Gfx_RequestLayerCopy(2);
    Palette_Flush();
    Mem_Copy32(data_027c0280->paletteBuffer, state->savedPalette, 0x400);
    Mem_Zero32(0, data_027c0280->paletteBuffer, 0x100);
    Ov004_LoadObjGraphics(state);
    Ov004_CreateSlotTasks(state);
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);

    Ov004_SetupOrtho3d();
    state->unk_8f8 = 0;
    Ov004_ClearSlots(state);
    task->frame = 0;
    task->duration = 1;
    task->step = 0;
}

#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"

void Screen_InitLayers(const GfxLayer *config);
void Screen_SetBgMode(int bgMode);
void func_01ff8a00(u16 *entries, u16 byteCount, int addend);
void Bg_FillRect(u16 *map, int x, int y, int width, int height, int tile);
BOOL func_01ff8ef8(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);
void Mem_Copy32(const void *src, void *dst, u32 size);
void Mem_Zero32(u32 value, void *dst, u32 size);
void Ov004_SetupPerspective3d(void);
void Ov004_ClearSlots(Unk_ov004_022bf158 *state);
void Ov004_UnpackResource(const char *name, void *dest);
void Ov004_CopyResource(const char *name, void *dest);

extern Unk_ov004_022bf158 *data_ov004_022bf158;
extern GfxLayer data_ov004_022be8b8[4];
extern char data_ov004_022bdd54[];      /* "096": layer 3 tiles */
extern char data_ov004_022bde00[];      /* "098": layer 3 map */
extern char data_ov004_022bdda0[];      /* "097": layer 3 palette */
extern char data_ov004_022bddc0[];      /* "077": layer 1 and 2 tiles */
extern char data_ov004_022bddbc[];      /* "078": layer 1 and 2 map */
extern char data_ov004_022bddb8[];      /* "070": palette */
extern u32 data_ov004_022bdc94[];       /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84[];       /* map bytes of a rotation/scaling BG by screen size */

/* Size in bytes of the tile map of a layer (as in Ov004_LoadBgLayers1And2). */
#define LAYER_MAP_BYTES(layer, i)                                               \
    (func_01ff8ef8(i) ? data_ov004_022bdc94[(layer)->screenSize]                \
     : Screen_IsAffineBg(i) ? data_ov004_022bdc84[(layer)->screenSize]          \
                            : data_ov004_022bdc84[(layer)->screenSize] * 2)

/* Script step: sets up the main screen like Ov004Task_SetupLayer3Scene (only
 * BG3 shown, picture 096/098 in BG palette 8, which starts black), then loads
 * picture 077/078 into layers 1 and 2 with palette 070 in BG palette 0.
 * Layer 1 starts scrolled 256 pixels right. */
void Ov004Task_SetupLayer3SceneWithLayers1And2(Ov004Task *task) {
    Unk_ov004_022bf158 *state = data_ov004_022bf158;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedScreen;
    u16 *map;

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Screen_SetBgMode(0);
    Screen_InitLayers(data_ov004_022be8b8);
    data_027c0280->dispCnt &= ~0x100;
    data_027c0280->dispCnt &= ~0x200;
    data_027c0280->dispCnt &= ~0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt &= ~0x1000;
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->dispCnt &= ~0x2000;
    data_027c0280->dispCnt &= ~0x4000;
    data_027c0280->dispCnt &= ~0x8000;
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBackdrop = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 1;
    data_027c0280->blendBottomBackdrop = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendBottomObj = 0;
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 0;
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;

    Ov004_UnpackResource(data_ov004_022bdd54, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14));
    Ov004_UnpackResource(data_ov004_022bde00, GFX_LAYER(data_027c0280, 3)->map);
    Ov004_CopyResource(data_ov004_022bdda0, data_027c0280->paletteBuffer + 0x100);
    map = GFX_LAYER(data_027c0280, 3)->map;
    func_01ff8a00(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 3), 3), 0x8000);
    Gfx_RequestLayerCopy(3);
    Palette_Flush();
    Mem_Copy32(data_027c0280->paletteBuffer, state->savedPalette, 0x400);
    Mem_Zero32(0, data_027c0280->paletteBuffer + 0x100, 0x100);

    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 256;
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;

    Ov004_UnpackResource(data_ov004_022bddc0, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14));
    Ov004_UnpackResource(data_ov004_022bddbc, GFX_LAYER(data_027c0280, 1)->map);
    map = GFX_LAYER(data_027c0280, 1)->map;
    func_01ff8a00(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 1), 1), 0);
    /* clear the second 32x32 screen block of layer 1's map */
    Bg_FillRect(GFX_LAYER(data_027c0280, 1)->map + 0x400, 0, 0, 32, 32, 0);
    Gfx_RequestLayerCopy(1);

    Ov004_UnpackResource(data_ov004_022bddc0, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
    Ov004_UnpackResource(data_ov004_022bddbc, GFX_LAYER(data_027c0280, 2)->map);
    map = GFX_LAYER(data_027c0280, 2)->map;
    func_01ff8a00(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 2), 2), 0);
    Bg_FillRect(GFX_LAYER(data_027c0280, 2)->map, 0, 0, 32, 32, 0);
    Gfx_RequestLayerCopy(2);
    Ov004_CopyResource(data_ov004_022bddb8, data_027c0280->paletteBuffer);
    Palette_Flush();
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);

    state->unk_8f8 = 0;
    state->unk_8fc = 0;
    state->unk_900 = 0;
    state->unk_904 = 0;
    Ov004_SetupPerspective3d();
    state->unk_908 = -128;
    state->unk_90c = 0;
    Ov004_ClearSlots(state);
    task->frame = 0;
    task->duration = 0;
    task->step = 0;
}

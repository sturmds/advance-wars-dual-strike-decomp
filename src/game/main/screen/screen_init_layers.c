#include "types.h"
#include "game/BgControl.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 *data_02297790;

void BgControl_SetCharBase(BgControl *bg, u32 charOffset);
void BgControl_SetScreenBase(BgControl *bg, u32 offset);
void BgControl_SetScreenSize(BgControl *bg, int screenSize);
void Screen_ResetBgRegisters(void);                /* resets the screen's graphics state */
void Screen_SetBg0Mode(int bg0Is3d);         /* sets Unk_027c0280.bg0Mode and applies the graphics mode */
BOOL func_01ff8ef8(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_ClearLayerMap(int layer);
void Gfx_ClearLayerBlankTile(int layer);
void func_01ff8d94(int layer);

/* BGxCNT of the BG that layer i uses. */
#define LAYER_BG_CNT(i) (data_027c0280->bgCnt[GFX_LAYER(data_027c0280, i)->bg])

/* Sets up the four layers of the current screen from config[0..3]: copies
 * each into the screen state, sets its BG's character base, screen base and
 * size, and turns on all five display layers. Layers of mode GFX_LAYER_NO_COPY
 * make BG0 a 3D layer. */
void Screen_InitLayers(const GfxLayer *config)
{
    int has3dLayer;
    int i;
    GfxLayer *layer;

    Screen_ResetBgRegisters();
    has3dLayer = FALSE;
    for (i = 0; i < 4; i++, config++) {
        layer = GFX_LAYER(data_027c0280, i);
        layer->mode = config->mode;
        layer->bg = config->bg;
        layer->charBase = config->charBase;
        layer->screenBase = config->screenBase;
        layer->screenSize = config->screenSize;
        layer->map = config->map;
        if (layer->mode == GFX_LAYER_NO_COPY) {
            has3dLayer = TRUE;
            BgControl_SetCharBase(&LAYER_BG_CNT(i), layer->charBase);
            BgControl_SetScreenBase(&LAYER_BG_CNT(i), layer->screenBase);
            BgControl_SetScreenSize(&LAYER_BG_CNT(i), layer->screenSize);
        } else {
            BgControl_SetCharBase(&LAYER_BG_CNT(i), layer->charBase);
            BgControl_SetScreenBase(&LAYER_BG_CNT(i), layer->screenBase);
            BgControl_SetScreenSize(&LAYER_BG_CNT(i), layer->screenSize);
            if (func_01ff8ef8(i) == 1) {
                if (layer->mode == 1) {
                    LAYER_BG_CNT(i).colorMode = 1;
                } else {
                    LAYER_BG_CNT(i).colorMode = 0;
                }
            } else if (Screen_IsAffineBg(i) == 1) {
                LAYER_BG_CNT(i).colorMode = 1;
            }
            Gfx_ClearLayerMap(i);
            Gfx_ClearLayerBlankTile(i);
            func_01ff8d94(i);
        }
    }
    if (data_027c027c == data_027c0288.unk_00) {
        if (has3dLayer) {
            Screen_SetBg0Mode(1);
        } else {
            Screen_SetBg0Mode(0);
        }
    }
    data_027c0280->dispCnt |= 0x100;
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->bgScreenOffset = 0;
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 0;
    data_027c0280->dispCnt |= 0x20;
    {
        u8 *vram = data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14);

        /* Remembers layer 0's tile data address when it lies in main-engine BG VRAM. */
        if ((u32)vram < 0x6200000) {
            data_02297790 = vram;
        }
    }
}

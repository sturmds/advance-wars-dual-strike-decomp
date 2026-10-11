#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

s32 HwDiv32(s32 numer, s32 denom);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void Palette_LoadFromFile(const char *fileName, int fileOffset, int paletteOffset, int size);
void Palette_Load(const void *src, u32 offset, u32 size);
const void *Terrain_GetPalette(int id, int army);
void Sound_PlaySe(u32 soundId);
void Text_PrintImmediate(int x, int y, void *target, u32 text, int a, int b);
void Gfx_RequestLayerCopy(int layer);
void func_0201e6e8(int x, int y, int width, int height);
void Bg_FillRect(u16 *map, int x, int y, int width, int height, int tile);
void Ov006_LoadCoIcons(int player);
void Ov006_StartSlotsSlideIn(void);
void Ov006_PostRequest(int request);
void Ov006_SetUpDarkenWindow(int a, int b, int c, int d);
void Ov006_LoadArmyTiles(int arg);
void func_ov006_023526c8(int arg);
void func_ov006_02352708(int arg);
void func_ov006_02355444(void);
void func_ov006_02355514(void);
void func_ov006_02355610(void);
void Ov006_AddToolButtons(void);
void func_ov006_02355b44(void);
void func_ov006_02355bd4(void);
void Ov006_AddEntryButtons(void);

extern Unk_ov006_0235f690 *data_ov006_0236d640;
extern s16 gSineTable[];                /* looks like a sine table: index 64 = cosine of 0 */
/* Matrix scales per frame (256 = 1x): 256, 224, 160, 112, 16 (zoom out or
 * squash), then 0, 0, 0, 0, 128, 224, 256 (way back), then -1. */
extern s16 Ov006_gPanelScaleSteps[];
extern const char data_ov006_0236b8a4[];   /* "/mapedit" */
extern const char data_ov006_0236b8b0[];   /* "002" */
extern const char data_ov006_0236b8b4[];   /* "001" */

#define OV6 data_ov006_0236d640
#define GFX data_027c0280

/* Divisor for one matrix entry: the scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Sets OBJ affine matrix 0 to scale by scaleX / 256 and scaleY / 256.
 * Needed to match: the divisors are evaluated before the table reads, and
 * the two locals are declared sine first. */
#define SET_MATRIX0(scaleX, scaleY)                                                    \
    {                                                                                  \
        int d3 = DIVISOR(scaleY);                                                      \
        int d2 = DIVISOR(scaleX);                                                      \
        int d1 = DIVISOR(scaleY);                                                      \
        int d0 = DIVISOR(scaleX);                                                      \
        int sinValue;                                                                  \
        int cosValue;                                                                  \
                                                                                       \
        cosValue = gSineTable[64] * 16;                                             \
        sinValue = gSineTable[0];                                                   \
                                                                                       \
        Oam_SetAffineParams(0, HwDiv32(cosValue, d0), HwDiv32(-sinValue * 16, d1),           \
                      HwDiv32(sinValue * 16, d2), HwDiv32(cosValue, d3));              \
    }

#define COPY_WINDOW_RECT()                                                             \
    {                                                                                  \
        GFX->win0Left = OV6->winLeft;                                                  \
        GFX->win0Top = OV6->winTop;                                                    \
        GFX->win0Right = OV6->winRight;                                                \
        GFX->win0Bottom = OV6->winBottom;                                              \
    }

#define START_WINDOW_RECT()                                                            \
    {                                                                                  \
        OV6->winLeft = OV6->unitMode == 1 ? 144 : 240;                                   \
        OV6->winTop = 16;                                                              \
        OV6->winRight = OV6->winLeft + 1;                                              \
        OV6->winBottom = OV6->winTop + 1;                                              \
        COPY_WINDOW_RECT();                                                            \
    }

/* Per-frame update of the panel shown while request 2 is active (the caller
 * runs it for that request). OV6->screenState is its state:
 *   bit 0x8000 of unk_00: reset; darken layers 1-3 and sprites outside
 *     window 0, load graphics from "/mapedit" ("002" into OBJ VRAM, palette
 *     "001").
 *   0-4: load the page's palette and graphics, then grow window 0 from a
 *     point at x 144 or 240 to (0, 20)-(255, 159), add the buttons and print
 *     text 0x7e000035. 51: idle.
 *   70-74: switch page (unitMode: 0 army, 1 CO): OBJ matrix 0 zooms out,
 *     the other page loads, then it zooms back in.
 *   60-62: close: clear the buttons, shrink the window, undo the blend, slide
 *     the slot buttons back in and post request 1.
 *   30-32: squash OBJ matrix 0 flat, reload the chosen army or CO icons,
 *     and widen it again. */
void Ov006_UpdateArmyCoPanel(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    int done;
    int scale;

    if (OV6->unk_00 & 0x8000) {
        OV6->unk_00 &= ~0x8000;
        OV6->screenState = 0;
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        Ov006_SetUpDarkenWindow(0, 256, 192, 192);
        GFX->dispCnt |= 0x2000;
        GFX->dispCnt &= ~0x4000;
        GFX->dispCnt &= ~0x8000;
        GFX->blendTopBg[GFX->layer0Bg] = 0;
        GFX->blendTopBg[GFX->layer1Bg] = 1;
        GFX->blendTopBg[GFX->layer2Bg] = 1;
        GFX->blendTopBg[GFX->layer3Bg] = 1;
        GFX->blendTopObj = 1;
        GFX->blendBottomBg[GFX->layer0Bg] = 0;
        GFX->blendBottomBg[GFX->layer1Bg] = 1;
        GFX->blendBottomBg[GFX->layer2Bg] = 0;
        GFX->blendBottomBg[GFX->layer3Bg] = 1;
        GFX->blendBottomObj = 1;
        GFX->blendMode = BLEND_MODE_DARKEN;
        GFX->blendEva = 16;
        GFX->blendEvb = 0;
        GFX->blendEvy = 9;
        START_WINDOW_RECT();
        FS_ChangeDir(data_ov006_0236b8a4);
        File_UnpackTo(GFX->objVram + 0x4940, data_ov006_0236b8b0);
        Palette_LoadFromFile(data_ov006_0236b8b4, 0, 0x200, 0x40);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }

    switch (OV6->screenState) {
    case 0:
        Palette_Load(Terrain_GetPalette(20, OV6->terrainPage), 0x300, 32);
        func_ov006_02352708(OV6->unitMode);
        OV6->screenState = 1;
        break;
    case 1:
        func_ov006_023526c8(OV6->unitMode);
        OV6->screenState = 2;
        break;
    case 2:
        START_WINDOW_RECT();
        Sound_PlaySe(0x1000a);
        OV6->screenState = 3;
    case 3:
        done = 0;
        OV6->winLeft += (-10 - OV6->winLeft) / 2;
        if (OV6->winLeft <= 0) {
            OV6->winLeft = 0;
            done++;
        }
        OV6->winRight += (263 - OV6->winRight) / 3;
        if (OV6->winRight >= 255) {
            OV6->winRight = 255;
            done++;
        }
        OV6->winTop += (28 - OV6->winTop) / 3;
        if (OV6->winTop >= 20) {
            OV6->winTop = 20;
            done++;
        }
        OV6->winBottom += (167 - OV6->winBottom) / 3;
        if (OV6->winBottom >= 159) {
            OV6->winBottom = 159;
            done++;
        }
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        COPY_WINDOW_RECT();
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
        if (done >= 4) {
            OV6->screenState = 4;
            saved0280 = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_00);
            saved027c = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_00);
            GFX->win0Left = 0;
            GFX->win0Top = 20;
            GFX->win0Right = 255;
            GFX->win0Bottom = 159;
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);
        }
        break;
    case 4:
        OV6->screenState = 51;
        Ov006_AddEntryButtons();
        func_ov006_02355bd4();
        Ov006_AddToolButtons();
        OV6->unk_02 |= 0x10;
        if (OV6->unitMode == 1) {
            OV6->unk_02 &= ~1;
        } else if (OV6->unitMode == 0) {
            OV6->unk_02 |= 1;
        }
        func_ov006_02355514();
        func_0201e6e8(0, 20, 28, 4);
        Text_PrintImmediate(1, 21, GFX->layer0Map, 0x7e000035, 0, 0);
        Gfx_RequestLayerCopy(0);
        break;
    case 51:
        break;
    }

    switch (OV6->screenState) {
    case 70:
        SET_MATRIX0(256, 256);
        OV6->unk_02 |= 8;
        OV6->unk_3b = 0;
        OV6->screenState = 71;
    case 71:
        scale = Ov006_gPanelScaleSteps[OV6->unk_3b];
        SET_MATRIX0(scale, scale);
        OV6->unk_3b++;
        if (OV6->unk_3b >= 5) {
            func_ov006_02355b44();
            func_ov006_02355610();
            OV6->unk_02 &= ~0x10;
            Palette_Load(Terrain_GetPalette(20, OV6->terrainPage), 0x300, 32);
            func_ov006_02352708(OV6->unitMode);
            OV6->unk_02 &= ~8;
            OV6->screenState = 72;
        }
        break;
    case 72:
        func_ov006_023526c8(OV6->unitMode);
        OV6->screenState = 73;
        OV6->unk_3b = 4;
        break;
    case 73:
        if (OV6->unk_3b > 0) {
            OV6->unk_3b--;
            break;
        }
        Ov006_AddEntryButtons();
        func_ov006_02355bd4();
        Ov006_AddToolButtons();
        if (OV6->unitMode == 1) {
            OV6->unk_02 &= ~1;
        } else if (OV6->unitMode == 0) {
            OV6->unk_02 |= 1;
        }
        SET_MATRIX0(0, 0);
        OV6->unk_02 |= 8;
        OV6->unk_02 |= 0x10;
        OV6->unk_3b = 8;
        OV6->screenState = 74;
        break;
    case 74:
        scale = Ov006_gPanelScaleSteps[OV6->unk_3b];
        SET_MATRIX0(scale, scale);
        OV6->unk_3b++;
        if (Ov006_gPanelScaleSteps[OV6->unk_3b] < 0) {
            Sound_PlaySe(0x1000a);
            OV6->unk_02 &= ~8;
            OV6->screenState = 51;
        }
        break;
    }

    switch (OV6->screenState) {
    case 60:
        func_ov006_02355b44();
        func_ov006_02355610();
        func_ov006_02355444();
        OV6->unk_02 &= ~0x10;
        Bg_FillRect(GFX->layer0Map, 0, 20, 28, 4, 0);
        Bg_FillRect(GFX->layer1Map, 0, 20, 28, 4, 0);
        Gfx_RequestLayerCopy(0);
        Gfx_RequestLayerCopy(1);
        OV6->screenState = 61;
        Sound_PlaySe(0x10009);
    case 61:
        done = 0;
        OV6->winTop += (106 - OV6->winTop) / 3;
        if (OV6->winTop >= 96) {
            OV6->winTop = 96;
            done++;
        }
        OV6->winBottom += (86 - OV6->winBottom) / 3;
        if (OV6->winBottom <= 96) {
            OV6->winBottom = 97;
            done++;
        }
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        COPY_WINDOW_RECT();
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
        if (done < 2) {
            break;
        }
    case 62:
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        GFX->win0Top = 152;
        GFX->dispCnt &= ~0x2000;
        GFX->dispCnt &= ~0x4000;
        GFX->dispCnt &= ~0x8000;
        GFX->blendTopBg[GFX->layer0Bg] = 0;
        GFX->blendTopBg[GFX->layer1Bg] = 0;
        GFX->blendTopBg[GFX->layer2Bg] = 0;
        GFX->blendTopBg[GFX->layer3Bg] = 0;
        GFX->blendTopObj = 0;
        GFX->blendBottomBg[GFX->layer0Bg] = 0;
        GFX->blendBottomBg[GFX->layer1Bg] = 0;
        GFX->blendBottomBg[GFX->layer2Bg] = 0;
        GFX->blendBottomBg[GFX->layer3Bg] = 0;
        GFX->blendBottomObj = 0;
        GFX->blendMode = 0;
        GFX->blendEva = 0;
        GFX->blendEvb = 0;
        GFX->blendEvy = 0;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        GFX->win0Left = 0;
        GFX->win0Top = 0;
        GFX->win0Right = 1;
        GFX->win0Bottom = 1;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
        Ov006_StartSlotsSlideIn();
        Ov006_PostRequest(1);
        break;
    }

    switch (OV6->screenState) {
    case 30:
        OV6->unk_02 |= 4;
        OV6->squashStep = 0;
        OV6->screenState = 31;
        SET_MATRIX0(256, 256);
        break;
    case 31:
        scale = Ov006_gPanelScaleSteps[OV6->squashStep++];
        if (scale > 0) {
            SET_MATRIX0(scale, 256);
        } else if (scale == 0) {
            SET_MATRIX0(scale, 256);
            if (OV6->unitMode == 1) {
                Ov006_LoadCoIcons(OV6->unitPage);
            }
            if (OV6->unitMode == 0) {
                Ov006_LoadArmyTiles(OV6->terrainPage);
            }
            OV6->screenState = 32;
        }
        break;
    case 32:
        scale = Ov006_gPanelScaleSteps[OV6->squashStep++];
        if (scale >= 0) {
            SET_MATRIX0(scale, 256);
        }
        if (scale == -1) {
            OV6->unk_02 &= ~4;
            OV6->screenState = 51;
        }
        break;
    }
}

#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"

#define REG_DISPCNT (*(volatile u32 *)0x04000000)

void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void *config);
void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void File_ReadTo(void *dest, const char *fileName);
void Mem_Fill32(u32 value, void *dst, u32 size);
void DC_FlushRange(const void *start, u32 size);  /* guess: flushes the data cache for the range */
void func_0210cb04(void);
void func_0210c9a0(const void *src, u32 offset, u32 size);  /* guess: copies to texture VRAM */
void func_0210c930(void);
void func_0210c8e8(void);
void GX_LoadTexPltt(const void *src, u32 offset, u32 size);  /* guess: copies to texture palette VRAM */
void GX_EndLoadTexPltt(void);
void Ov004_SetupPerspective3d(void);
void Ov004_LoadBottomSpriteGraphics(Unk_ov004_022bf158 *state);
void Ov004_InitVramBanksInOrder(void);
void Ov004_LoadSpriteGraphicsBothScreens(Unk_ov004_022bf158 *state);
void Ov004_LoadBgLayer0(Unk_ov004_022bf158 *state);
void func_ov004_022bdc08(Unk_ov004_022bf158 *state);
/* These two are defined without parameters; the original call still passes
 * the state. */
void func_ov004_022bd978(Unk_ov004_022bf158 *state);
void Ov004_LoadBgLayers1And2(Unk_ov004_022bf158 *state);

extern u8 data_ov004_022bf03c[];        /* main screen layer config */
extern u8 data_ov004_022bf08c[];        /* sub screen layer config */
extern char data_ov004_022bf0dc[];      /* "/rikiishi" */
extern char data_ov004_022bf0e8[];      /* "0c6": texture */
extern char data_ov004_022bf0ec[];      /* "0c8": texture palette */
extern TaskScriptCmd data_ov004_022befd4[];
extern TaskScriptCmd data_ov004_022befbc[];

/* Script step that sets up both screens with alpha blending (main screen:
 * layer 1 over layers 1 and 2 at 14/4; sub screen: layer 1 over layer 2 at
 * 2/8), loads texture "0c6" and palette "0c8" into 3D VRAM with the upper
 * half of the palette filled with 0xffff, starts the two scene tasks at unk_1bf4 and
 * unk_1c04, then loads the layers of both screens. Goes on at step 20 when
 * state->unk_008 is set, else at step 0. */
void Ov004Task_InitBlendedScene(Ov004Task *task)
{
    Unk_ov004_022bf158 *state = task->state;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;
    u8 *buffer;

    Ov004_InitVramBanksInOrder();

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Screen_SetBgMode(0);
    Screen_InitLayers(data_ov004_022bf03c);
    data_027c0280->dispCnt |= 0x100;
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt &= ~0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->dispCnt &= ~0x2000;
    data_027c0280->dispCnt &= ~0x4000;
    data_027c0280->dispCnt &= ~0x8000;
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 14;
    data_027c0280->blendEvb = 4;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBackdrop = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBackdrop = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
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
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Screen_SetBgMode(0);
    Screen_InitLayers(data_ov004_022bf08c);
    data_027c0280->dispCnt &= ~0x100;
    data_027c0280->dispCnt &= ~0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt &= ~0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->dispCnt &= ~0x2000;
    data_027c0280->dispCnt &= ~0x4000;
    data_027c0280->dispCnt &= ~0x8000;
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 2;
    data_027c0280->blendEvb = 8;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBackdrop = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBackdrop = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
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
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);

    REG_DISPCNT &= ~0x7000000;
    FS_ChangeDir(data_ov004_022bf0dc);
    buffer = state->buffer;
    File_UnpackTo(buffer, data_ov004_022bf0e8);
    DC_FlushRange(buffer, 0x8000);
    func_0210cb04();
    func_0210c9a0(buffer, 0, 0x8000);
    func_0210c930();
    File_ReadTo(buffer, data_ov004_022bf0ec);
    Mem_Fill32(-1, buffer + 0x200, 0x200);
    DC_FlushRange(buffer, 0x400);
    func_0210c8e8();
    GX_LoadTexPltt(buffer, 0, 0x400);
    GX_EndLoadTexPltt();
    Ov004_SetupPerspective3d();
    Ov004_LoadSpriteGraphicsBothScreens(state);
    Ov004_LoadBottomSpriteGraphics(state);
    state->unk_000 = 0;
    state->unk_1bf0 = 0;
    func_ov004_022bdc08(state);
    state->unk_1bf4 = Task_Create(data_ov004_022befd4, TASK_LIST(4));
    state->unk_1c04 = Task_Create(data_ov004_022befbc, TASK_LIST(4));

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    func_ov004_022bd978(state);
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Ov004_LoadBgLayer0(state);
    Ov004_LoadBgLayers1And2(state);
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);

    if (state->unk_008 != 0) {
        task->step = 20;
    } else {
        task->step = 0;
    }
}

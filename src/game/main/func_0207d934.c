#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "sdk/gx.h"
#include "game/text_print.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_021388f0[];
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetCurrentBoth(void *state);
void Screen_ApplyAll(void);
void Sound_StopSong(void);
void Sound_StopSubSe(int fadeFrames);
void Task_InitSystem(void);
void Menu_Reset(void);
void Gfx_RequestLayerCopy(int layer);
void Palette_Load(const void *src, int offset, u32 size);
void Mem_Copy32(const void *src, void *dst, u32 size);
void Gfx_CopyRequestedLayers(void);
void func_01ff9c28(int a);
void MainLoop_SetFrameCallback(int a);
void TransferQueue_Clear(void);
void CallbackSlots_Clear(void);
void CallbackList_Clear(void);
void ScriptSlot_ClearAll(void);
void SpriteAnim_FreeAll(void);
void Keypad_Init(void);
void OamBuffer_ClearTail(void);
void OamBuffer_FlushTail(void);
void func_0201d7fc(void);
void Gfx_InitBothScreens(void);
#define MAIN_SCREEN (data_027c0288.unk_00)
#define SUB_SCREEN  (data_027c0288.unk_04)
#define RESET_SCREEN_STATE() \
    TransferQueue_Clear();         \
    CallbackSlots_Clear();         \
    CallbackList_Clear();         \
    ScriptSlot_ClearAll();         \
    Task_InitSystem();       \
    Menu_Reset();            \
    SpriteAnim_FreeAll();         \
    Keypad_Init();         \
    OamBuffer_ClearTail();         \
    OamBuffer_FlushTail();         \
    func_0201d7fc();         \
    func_01ff9c28(0);        \
    MainLoop_SetFrameCallback(0)
#define CLEAR_WINDOW_BITS()               \
    data_027c0280->dispCnt &= ~0x2000;    \
    data_027c0280->dispCnt &= ~0x4000;    \
    data_027c0280->dispCnt &= ~0x8000

void func_0207d934(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(MAIN_SCREEN);
    data_027c0280->masterBrightness = -16;
    Screen_SetCurrentBoth(SUB_SCREEN);
    data_027c0280->masterBrightness = -16;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, -16);
    GXx_SetMasterBrightness_(IO_SUB_SCREEN_FADE, -16);
    Sound_StopSong();
    Sound_StopSubSe(0);

    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(MAIN_SCREEN);
    RESET_SCREEN_STATE();
    Screen_SetCurrentBoth(SUB_SCREEN);
    RESET_SCREEN_STATE();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    Gfx_InitBothScreens();

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(MAIN_SCREEN);
    saved027c = data_027c027c;
    Screen_SetCurrent(MAIN_SCREEN);
    Text_PrintId(0, 9, data_027c0280->layer0Map, 0x48000008, 0, 0x100);
    Gfx_RequestLayerCopy(0);
    Palette_Load(data_021388f0, 0, 32);
    Gfx_CopyRequestedLayers();
    Mem_Copy32(data_027c0280->paletteBuffer, data_027c0280->paletteCopy, 0x400);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(SUB_SCREEN);
    saved027c = data_027c027c;
    Screen_SetCurrent(SUB_SCREEN);
    Gfx_CopyRequestedLayers();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(MAIN_SCREEN);
    data_027c0280->masterBrightness = 0;
    Screen_SetCurrentBoth(SUB_SCREEN);
    data_027c0280->masterBrightness = 0;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(MAIN_SCREEN);
    CLEAR_WINDOW_BITS();
    Screen_SetCurrentBoth(SUB_SCREEN);
    CLEAR_WINDOW_BITS();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    Screen_ApplyAll();
}

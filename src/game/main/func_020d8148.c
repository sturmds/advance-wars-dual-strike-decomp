#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetCurrentBoth(void *state);
void Screen_ResetWindows(void);
void CallbackSlots_Clear(void);
void func_01ff9c28(void (*callback)(void));
void MainLoop_SetFrameCallback(void (*callback)(void));
void MainLoop_DefaultVBlank(void);
void MainLoop_TaskFrame(void);
void Gfx_InitBothScreens(void);
void func_020044a8(void);
void Wallpaper_LoadBoth(void);
void ObjGfx_LoadLanguageTiles(void);
void Font_LoadDigits(int baseTile);
void Gfx_LoadPlayerWindowFrame(int player);
void Palette_Load(const void *src, u32 offset, u32 size);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const u8 data_02139710[];

/* Window reset, then the whole screen darkened to black. */
#define DARKEN_SCREEN(state)                              \
    Screen_SetCurrentBoth(state);                         \
    Screen_ResetWindows();                                \
    data_027c0280->blendMode = BLEND_MODE_DARKEN;         \
    data_027c0280->blendEva = 0;                          \
    data_027c0280->blendEvb = 0;                          \
    data_027c0280->blendEvy = 16

/* A step of the task scripts data_021675e0 and data_021676a0. Same as
 * func_020d7c08 without the task it starts: darkens both screens, resets
 * the frame callbacks, loads the wallpapers, the digit font, the window
 * frame and BG palette 1. */
void func_020d8148(void)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c = data_027c027c;

    DARKEN_SCREEN(data_027c0288.unk_00);
    DARKEN_SCREEN(data_027c0288.unk_04);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    CallbackSlots_Clear();
    MainLoop_SetFrameCallback(MainLoop_TaskFrame);
    func_01ff9c28(MainLoop_DefaultVBlank);
    Gfx_InitBothScreens();
    func_020044a8();
    Wallpaper_LoadBoth();
    ObjGfx_LoadLanguageTiles();
    Font_LoadDigits(0);
    Gfx_LoadPlayerWindowFrame(0);
    Palette_Load(data_02139710, 32, 32);
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "sdk/gx.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_ov000_023503a0;
extern u8 data_02291774[];
extern u8 data_02166dc8[];

void MainLoop_Init(void);
void Boot_EnableDisplay(void);
void func_02112bdc(int a);
void Mem_InitHeaps(void);
void Screens_InitContexts(void);
void Screens_InitDisplay(void);
void Sound_InitSystem(void);
void Sound_Init(void);
void Sound_LoadResidentData(void);
void Overlay_ClearLoadedFlags(void);
void Overlay_Load(int slot);
void func_0200c0b0(void);
u8 OwnerInfo_GetFavoriteColor(void);
void Link_Init(void);
void Save_PackProgress(void);
void Save_ResetProgress(void);
void Save_Init(void (*a)(void), void (*b)(void), void *c);
void Touch_Init(void);
void Menu_Reset(void);
void GfxQueue_Init(void);
void Save_LoadAndUnpackProgress(void);
void DesignMap_RefreshAllRows(void);
void Rng_SetState(u32 state);
void VBlankIntrWait(void);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void func_020097a4(void);
void Screen_ApplyAll(void);
void Screen_InitSystems(void);
void Screens_SetUp2dLayers(const void *unused);
void NumberFont_LoadLow(void);
void func_01ff9c28(void (*callback)(void));
void MainLoop_SetFrameCallback(void (*callback)(void));
void MainLoop_DefaultVBlank(void);
void MainLoop_TaskFrame(void);
BOOL Boot_TryStartKeyComboScreen(u16 keys);
void func_02001250(void);
void func_01ff9c7c(void);

#define REG_KEYINPUT     (*(volatile u16 *)0x04000130)
#define HW_BUTTON_XY_BUF (*(volatile u16 *)0x027fffa8)  /* X, Y and debug bits, written by the ARM7 */
#define PAD_ALL_MASK     0x2fff

/* Sets up one screen: its per-screen state, then the frame callbacks. */
#define INIT_SCREEN(screen)                     \
    savedGfx = data_027c0280;                   \
    Screen_SetCurrentGfx(screen);               \
    savedState = data_027c027c;                 \
    Screen_SetCurrent(screen);                  \
    func_020097a4();                            \
    Screen_ApplyAll();                          \
    Screen_InitSystems();                            \
    Screens_SetUp2dLayers(data_02166dc8);               \
    NumberFont_LoadLow();                            \
    func_01ff9c28(MainLoop_DefaultVBlank);      \
    MainLoop_SetFrameCallback(MainLoop_TaskFrame);               \
    Screen_SetCurrent(savedState);              \
    Screen_SetCurrentGfx(savedGfx)

/* Game entry point, called from Entry. Sets up the hardware and both
 * screens, starts the first task unless Boot_TryStartKeyComboScreen took the keys held at
 * boot (L + Select + Right without Start or R), then runs the main loop
 * forever. */
void main(void)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;
    volatile u16 heldKeys;  /* volatile to match: the original keeps it on the stack */

    MainLoop_Init();
    Boot_EnableDisplay();
    /* KEYINPUT plus the X/Y bits, inverted: keys are active low */
    heldKeys = ((REG_KEYINPUT | HW_BUTTON_XY_BUF) ^ PAD_ALL_MASK) & PAD_ALL_MASK;
    func_02112bdc(3);
    Mem_InitHeaps();
    Screens_InitContexts();
    Screens_InitDisplay();
    Sound_InitSystem();
    Sound_Init();
    Sound_LoadResidentData();
    Overlay_ClearLoadedFlags();
    Overlay_Load(0);
    Overlay_Load(1);
    func_0200c0b0();
    data_ov000_023503a0 = OwnerInfo_GetFavoriteColor();
    Link_Init();
    Save_Init(Save_PackProgress, Save_ResetProgress, data_02291774);
    Touch_Init();
    Menu_Reset();
    GfxQueue_Init();
    Save_ResetProgress();
    Save_LoadAndUnpackProgress();
    DesignMap_RefreshAllRows();
    Rng_SetState(0x0a6b99cd);
    VBlankIntrWait();

    INIT_SCREEN(data_027c0288.unk_00);
    INIT_SCREEN(data_027c0288.unk_04);

    *REG_POWCNT1 &= ~POWCNT1_DISPLAY_SWAP;
    if (!Boot_TryStartKeyComboScreen(heldKeys)) {
        func_02001250();
    }
    while (1) {
        func_01ff9c7c();
    }
}

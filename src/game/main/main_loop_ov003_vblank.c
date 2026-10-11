#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

int func_01ff9f0c(void);
void func_01ff9f1c(int value);
void Ov003_WriteFxColors(void);
void Screen_SetCurrentBoth(void *state);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Task_RunList(Task *task);
void CallbackSlots_RunAll(void);
void OamBuffer_FlushHead(void);
void OamBuffer_FlushTail(void);
void Screen_ApplyAll(void);
void TransferQueue_Run(void);
void Gfx_CopyRequestedLayers(void);
void CallbackList_RunAndClear(void);
int Link_IsActive(void);
void func_0207d930(void);

/* Runs `body` on both screens, then puts the current screen back. */
#define ON_BOTH_SCREENS(body)                   \
    savedGfx = data_027c0280;                   \
    savedState = data_027c027c;                 \
    Screen_SetCurrentBoth(data_027c0288.unk_00); \
    body;                                       \
    Screen_SetCurrentBoth(data_027c0288.unk_04); \
    body;                                       \
    Screen_SetCurrent(savedState);              \
    Screen_SetCurrentGfx(savedGfx)

/* The gMainLoop callback Ov003_Init installs. Works like
 * MainLoop_DefaultVBlank, but always starts with Ov003_WriteFxColors and
 * switches screens with Screen_SetCurrentBoth. */
void MainLoop_Ov003VBlank(void)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;

    Ov003_WriteFxColors();
    ON_BOTH_SCREENS(Task_RunList(data_027c027c->taskLists[0]));
    CallbackSlots_RunAll();
    ON_BOTH_SCREENS(OamBuffer_FlushHead());
    if (func_01ff9f0c()) {
        func_01ff9f1c(0);
        ON_BOTH_SCREENS(OamBuffer_FlushTail());
        Screen_ApplyAll();
        TransferQueue_Run();
        ON_BOTH_SCREENS(Gfx_CopyRequestedLayers());
        CallbackList_RunAndClear();
    } else {
        func_01ff9f1c(0);
        if (Link_IsActive() == 1) {
            func_0207d930();
        }
    }
}

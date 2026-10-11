#include "types.h"
#include "game/Unk_02080440.h"
#include "game/menu_button.h"
#include "game/task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 data_0229eaec;

BOOL EventScript_IsRunning(void);
BOOL func_020d1b70(void);
u32 func_020ef1bc(void);
u32 MapTouch_ReadTileTapOrBack(void);
/* Defined without parameters; the original passes the task in r0. */
void func_020eef58(Task *task);
void Silo_StartLaunch(int siloX, int siloY, int targetX, int targetY, void *parentOrList);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void Task_ResumeScript(Task *task);
void Task_JumpToLabel(Task *task, int label);
void func_020f5888(int x, int y);
void func_020c4024(void);
void func_020c407c(void);
void MapView_ScrollStep(int a);
void func_020c4124(int a, int b);

/* Bits func_020ef1bc (buttons) and MapTouch_ReadTileTapOrBack (touch) return. */
#define INPUT_CONFIRM 0x10
#define INPUT_CANCEL  0x20

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Task step while the player aims a silo (the acting unit stands on it) at
 * the cursor tile (unk_28, unk_2a). Confirm launches it there and, when
 * data_0229eaec is set, sends action 22 for it; cancel puts the cursor back
 * on the silo and jumps the script to label 0. Otherwise the cursor and
 * the map view keep moving. */
void SiloLaunch_PickTargetStep(Task *task)
{
    u16 buttons;
    u16 input;

    if (EventScript_IsRunning()) {
        return;
    }
    if (func_020d1b70()) {
        buttons = func_020ef1bc();
        input = buttons | MapTouch_ReadTileTapOrBack();
        if (input & INPUT_CONFIRM) {
            func_020eef58(task);
            Silo_StartLaunch(GAME->actingUnitDestX, GAME->actingUnitDestY, GAME->unk_28, GAME->unk_2a, task);
            MenuButtons_CloseAll();
            if (data_0229eaec != 0) {
                UnitAction_Send(22, GAME->actingUnitIndex, GAME->unk_28, GAME->unk_2a);
            }
            Task_ResumeScript(task);
            return;
        }
        if (input & INPUT_CANCEL) {
            MenuButtons_CloseAll();
            GAME->unk_28 = GAME->actingUnitDestX;
            GAME->unk_2a = GAME->actingUnitDestY;
            func_020f5888(GAME->unk_28, GAME->unk_2a);
            Task_JumpToLabel(task, 0);
            Task_ResumeScript(task);
            return;
        }
    }
    func_020c4024();
    func_020c407c();
    MapView_ScrollStep(4);
    func_020c4124(2, 18);
}

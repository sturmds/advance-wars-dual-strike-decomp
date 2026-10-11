#include "types.h"
#include "game/Unk_02080440.h"
#include "game/menu_button.h"
#include "game/task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 data_0229eaec;

BOOL EventScript_IsRunning(void);
BOOL func_020d1b70(void);
u32 func_020ef190(void);
u32 func_020efc20(void);
void BlackBomb_RemoveActingUnit(Task *task);
void func_020dd940(int x, int y, int targetX, int targetY, void *parentOrList);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void Task_ResumeScript(Task *task);
void Task_JumpToLabel(Task *task, int label);
void func_020f5888(int x, int y);
void func_020c4124(int a, int b);

/* Bits func_020ef190 (buttons) and func_020efc20 (touch) return. */
#define INPUT_CONFIRM 0x10
#define INPUT_CANCEL  0x20

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Task step while the player confirms a Black Bomb explosion. Confirm
 * removes the bomb (BlackBomb_RemoveActingUnit), starts the explosion
 * (func_020dd940) from the bomb's tile at the cursor tile (unk_28, unk_2a)
 * and, when data_0229eaec is set, sends unit action 23 for it. Cancel puts
 * the cursor back on the bomb and jumps the script to label 0. Same shape
 * as SiloLaunch_PickTargetStep, but the cursor does not move here. */
void BlackBomb_ConfirmExplodeStep(Task *task)
{
    u16 buttons;
    u16 input;

    if (EventScript_IsRunning()) {
        return;
    }
    func_020c4124(2, 19);
    if (!func_020d1b70()) {
        return;
    }
    buttons = func_020ef190();
    input = buttons | func_020efc20();
    if (input & INPUT_CONFIRM) {
        BlackBomb_RemoveActingUnit(task);
        func_020dd940(GAME->actingUnitDestX, GAME->actingUnitDestY, GAME->unk_28, GAME->unk_2a, task);
        MenuButtons_CloseAll();
        if (data_0229eaec != 0) {
            UnitAction_Send(23, GAME->actingUnitIndex, 0, 0);
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
    }
}

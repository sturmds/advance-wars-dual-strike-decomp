#include "types.h"
#include "game/Unk_02080440.h"
#include "game/menu_button.h"
#include "game/task.h"

extern u32 gLinkPlay;

BOOL EventScript_IsRunning(void);
BOOL MapView_IsAtRest(void);
u32 func_020ef190(void);
u32 MapTouch_ReadCursorTileTap(void);
void BlackBomb_RemoveActingUnit(Task* task);
void BlackBombExplosion_Start(int x, int y, int targetX, int targetY,
                              void* parentOrList);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void Task_ResumeScript(Task* task);
void Task_JumpToLabel(Task* task, int label);
void func_020f5888(int x, int y);
void MapCursor_DrawTwoStyles(int a, int b);

/* Bits func_020ef190 (buttons) and MapTouch_ReadCursorTileTap (touch) return. */
#define INPUT_CONFIRM 0x10
#define INPUT_CANCEL 0x20

/* Task step while the player confirms a Black Bomb explosion. Confirm
 * removes the bomb (BlackBomb_RemoveActingUnit), starts the explosion
 * (BlackBombExplosion_Start) from the bomb's tile at the cursor tile (cursorX, cursorY)
 * and, when gLinkPlay is set, sends unit action 23 for it. Cancel puts
 * the cursor back on the bomb and jumps the script to label 0. Same shape
 * as SiloLaunch_PickTargetStep, but the cursor does not move here. */
void BlackBomb_ConfirmExplodeStep(Task* task) {
  u16 buttons;
  u16 input;

  if (EventScript_IsRunning()) {
    return;
  }
  MapCursor_DrawTwoStyles(2, 19);
  if (!MapView_IsAtRest()) {
    return;
  }
  buttons = func_020ef190();
  input = buttons | MapTouch_ReadCursorTileTap();
  if (input & INPUT_CONFIRM) {
    BlackBomb_RemoveActingUnit(task);
    BlackBombExplosion_Start(MAP_STATE->actingUnitDestX,
                             MAP_STATE->actingUnitDestY, MAP_STATE->cursorX,
                             MAP_STATE->cursorY, task);
    MenuButtons_CloseAll();
    if (gLinkPlay != 0) {
      UnitAction_Send(23, MAP_STATE->actingUnitIndex, 0, 0);
    }
    Task_ResumeScript(task);
    return;
  }
  if (input & INPUT_CANCEL) {
    MenuButtons_CloseAll();
    MAP_STATE->cursorX = MAP_STATE->actingUnitDestX;
    MAP_STATE->cursorY = MAP_STATE->actingUnitDestY;
    func_020f5888(MAP_STATE->cursorX, MAP_STATE->cursorY);
    Task_JumpToLabel(task, 0);
    Task_ResumeScript(task);
  }
}

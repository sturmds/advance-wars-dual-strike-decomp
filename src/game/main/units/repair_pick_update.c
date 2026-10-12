#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

u32 NeighborTilePicker_Update(int* direction, int mask);
void Task_Destroy(Task* task);
void Task_ResumeScript(Task* task);
void MenuDepth_Pop(void);
void UnitMenu_Open(void);
void UnitAction_Repair(int direction);

/* Bits NeighborTilePicker_Update returns: A and B. */
#define INPUT_CONFIRM 0x10
#define INPUT_CANCEL 0x20

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Script step of the repair tile picker (data_02166678), like
 * UnloadTask_PickTile: unk_64 holds the mask of directions with a unit to
 * repair and unk_66 the picked one. B closes the picker and puts the view
 * target back on the acting unit's start tile; A starts the repair
 * (UnitAction_Repair) and continues the script. */
void RepairPick_Update(Task* task) {
  int direction;
  u32 input;
  int mask = task->unk_64; /* needed to match: read before direction */

  direction = task->unk_66;
  input = NeighborTilePicker_Update(&direction, mask);
  task->unk_66 = direction;
  if (input & INPUT_CANCEL) {
    Task_Destroy(task);
    MenuDepth_Pop();
    UnitMenu_Open();
    STATE->cursorX = STATE->actingUnitStartX;
    STATE->cursorY = STATE->actingUnitStartY;
    return;
  }
  if (!(input & INPUT_CONFIRM)) {
    return;
  }
  UnitAction_Repair(task->unk_66);
  Task_ResumeScript(task);
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"
#include "game/unit_show_task.h"
#include "game/unit_show_entry.h"

extern Unk_02080440* data_027c027c;
extern u8 data_ov000_022db564[];
extern u8 data_ov000_022db574[];
extern u8 data_ov000_022db57c[];
extern u8 data_ov000_022db584[];
extern u8 data_ov000_022db58c[];
extern u8 data_ov000_022db594[];
extern u8 data_ov000_022db5a4[];
extern u8 data_ov000_022db5a8[];
extern u8 data_ov000_022db5ac[];
extern u8 data_ov000_022db5b4[];
extern u8 data_ov000_022db5bc[];
extern u8 data_ov000_022db5c4[];
extern u8 data_ov000_022db5cc[];
extern u8 data_ov000_022db5d4[];
extern u8 data_ov000_022db5dc[];
extern u8 data_ov000_022db5ec[];
extern u8 data_ov000_022db5f4[];
extern u8 data_ov000_022db5fc[];
extern u8 data_ov000_022db60c[];
extern u8 data_ov000_022db614[];
extern u8 data_ov000_022db61c[];
extern u8 data_ov000_022db624[];
extern u8 data_ov000_022db62c[];
extern u8 data_ov000_022db63c[];
extern u8 data_ov000_022db664[];
extern u8 data_ov000_022db694[];
void func_ov000_022add0c(void);
void func_ov000_022ae47c(void);
void func_ov000_022ae488(void);
extern u8 data_ov000_022db5a0[];
extern u8 data_ov000_022db5b0[];
extern u8 data_ov000_022db5e8[];
extern u8 data_ov000_022db638[];
extern u8 data_ov000_022db660[];
extern u8 data_ov000_022db684[];
extern u8 data_ov000_022db690[];
void func_ov000_022add3c(void);
void ShowEntryTask_Update(void);
extern u8 data_ov000_022db560[];
extern u8 data_ov000_022db568[];
extern u8 data_ov000_022db56c[];
extern u8 data_ov000_022db570[];
extern u8 data_ov000_022db578[];
extern u8 data_ov000_022db588[];
extern u8 data_ov000_022db590[];
extern u8 data_ov000_022db59c[];
extern u8 data_ov000_022db5b8[];
extern u8 data_ov000_022db5c0[];
extern u8 data_ov000_022db5c8[];
extern u8 data_ov000_022db5d0[];
extern u8 data_ov000_022db5d8[];
extern u8 data_ov000_022db5e4[];
extern u8 data_ov000_022db5f0[];
extern u8 data_ov000_022db5f8[];
extern u8 data_ov000_022db600[];
extern u8 data_ov000_022db604[];
extern u8 data_ov000_022db608[];
extern u8 data_ov000_022db610[];
extern u8 data_ov000_022db618[];
extern u8 data_ov000_022db620[];
extern u8 data_ov000_022db628[];
extern u8 data_ov000_022db634[];
extern u8 data_ov000_022db65c[];
extern u8 data_ov000_022db680[];
extern u8 data_ov000_022db68c[];
extern u8 data_ov000_022db580[];
extern u8 data_ov000_022db598[];
extern u8 data_ov000_022db5e0[];
extern u8 data_ov000_022db630[];
extern u8 data_ov000_022db658[];
extern u8 data_ov000_022db67c[];
extern u8 data_ov000_022db688[];
extern u8 data_ov000_022db654[];
extern u8 data_ov000_022db678[];
extern u8 data_ov000_022db650[];
extern u8 data_ov000_022db674[];
extern u8 data_ov000_022db64c[];
extern u8 data_ov000_022db670[];
extern u8 data_ov000_022db648[];
extern u8 data_ov000_022db66c[];
extern u8 data_ov000_022db644[];
extern u8 data_ov000_022db668[];
extern u8 data_ov000_022db640[];

TaskScriptCmd data_ov000_022db70c[] = {
#include "data/ov000/data_ov000_022db70c.inc.c"
};
static TaskScriptCmd data_ov000_022db6e4[] = {
#include "data/ov000/data_ov000_022db6e4.inc.c"
};
UnitShowEntry gUnitShowEntries[] = {
#include "data/ov000/gUnitShowEntries.inc.c"
};

/* Starts a show task for a unit type at a tile, with no unit behind it
 * (task->unit is NULL). Does nothing while both show slots are busy. */
void UnitShowTask_CreateAtTile(int tileX, int tileY, int unitType) {
  int slot;
  UnitShowTask* task;

  for (slot = 0; slot < UNIT_SHOW_SLOT_COUNT; slot++) {
    if (data_027c027c->unk_58[slot] == 0) {
      break;
    }
  }
  if (slot == UNIT_SHOW_SLOT_COUNT) {
    return;
  }
  task = (UnitShowTask*)Task_Create(data_ov000_022db6e4, TASK_LIST(5));
  task->x = tileX << 4;
  task->y = tileY << 4;
  task->state = UNIT_SHOW_STATE_MOVE;
  task->entry = unitType;
  task->unit = NULL;
}

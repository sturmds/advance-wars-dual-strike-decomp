#include "types.h"
#include "game/unit_show_task.h"

void Sound_StopSubSe(int);

void func_ov000_022ae488(UnitShowTask* task) {
  Sound_StopSubSe(0);
  ShowEntry_Finish(task);
}

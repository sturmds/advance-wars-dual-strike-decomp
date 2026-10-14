#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"

extern TaskScriptCmd data_02169dac[];

void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void func_020f79a4(void);
void Task_DestroyByScript(void* script);

/* Destroys the data_02169dac task. When the current screen is not the first
 * one, it first clears layer 0 and runs the screen config's unk_38 callback. */
void func_020f77bc(void) {
  if (data_027c027c != data_027c0288.unk_00) {
    Gfx_ClearLayerMap(0);
    Gfx_RequestLayerCopy(0);
    func_020f79a4();
    if (data_027c027c->unk_50[data_027c0284]->unk_38 != NULL) {
      data_027c027c->unk_50[data_027c0284]->unk_38();
    }
  }
  Task_DestroyByScript(data_02169dac);
}

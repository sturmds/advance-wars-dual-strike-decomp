#include "types.h"
#include "game/task.h"
#include "game/Unk_ov006_0235f690.h"

void* EventScript_Start(const void* script);
int func_ov006_023580c0(void);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern const TaskScriptCmd data_ov006_0236a258[];
extern const TaskScriptCmd data_ov006_0236a308[];

/* After a posted request (flag 0x8000), waits 30 frames, then starts one of two
 * event scripts depending on func_ov006_023580c0. Runs once per request. */
void Ov006_StartScriptAfterDelay(void) {
  if (data_ov006_0236d640->unk_00 & 0x8000) {
    data_ov006_0236d640->unk_00 &= ~0x8000;
    data_ov006_0236d640->screenState = 0;
    data_ov006_0236d640->unk_3b = 30;
  }
  if (data_ov006_0236d640->screenState != 0) {
    return;
  }
  data_ov006_0236d640->unk_3b--;
  if (data_ov006_0236d640->unk_3b != 0) {
    return;
  }
  data_ov006_0236d640->screenState = 1;
  EventScript_Start(func_ov006_023580c0() == 0 ? data_ov006_0236a258
                                               : data_ov006_0236a308);
}

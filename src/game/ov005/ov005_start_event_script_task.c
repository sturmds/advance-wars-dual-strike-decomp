#include "types.h"
#include "game/event_script.h"

void func_02014c38(void* parentOrList);
EventScriptSlot* EventScript_Start(const EventScriptCommand* script);

extern int data_ov005_0236bbb4;

void Ov005_StartEventScriptTask(const EventScriptCommand* script,
                                void* parent) {
  data_ov005_0236bbb4 = 1;
  func_02014c38(parent);
  EventScript_Start(script);
}

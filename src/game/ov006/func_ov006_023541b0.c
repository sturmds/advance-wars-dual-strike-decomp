#include "types.h"
#include "game/event_script.h"

void Ov006_PrepareScriptScreen(void);
extern EventScriptCommand data_ov006_0236b1fc[];

int func_ov006_023541b0(void) {
  Ov006_PrepareScriptScreen();
  return (int)EventScript_Start(data_ov006_0236b1fc);
}

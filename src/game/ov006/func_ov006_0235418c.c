#include "types.h"
#include "game/event_script.h"

void Ov006_PrepareScriptScreen(void);
extern EventScriptCommand data_ov006_0236b2ac[];

int func_ov006_0235418c(void) {
  Ov006_PrepareScriptScreen();
  return (int)EventScript_Start(data_ov006_0236b2ac);
}

#include "types.h"
#include "game/event_script.h"

void Ov006_PrepareScriptScreen(void);
extern EventScriptCommand data_ov006_0236b4bc[];

int func_ov006_023541d4(void) {
  Ov006_PrepareScriptScreen();
  return (int)EventScript_Start(data_ov006_0236b4bc);
}

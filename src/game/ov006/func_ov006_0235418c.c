#include "types.h"

void Ov006_PrepareScriptScreen(void);
int EventScript_Start(void*);
extern u8 data_ov006_0236b2ac[];

int func_ov006_0235418c(void) {
  Ov006_PrepareScriptScreen();
  return EventScript_Start(data_ov006_0236b2ac);
}

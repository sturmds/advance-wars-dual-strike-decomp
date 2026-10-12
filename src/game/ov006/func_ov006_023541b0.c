#include "types.h"

void Ov006_PrepareScriptScreen(void);
int EventScript_Start(void*);
extern u8 data_ov006_0236b1fc[];

int func_ov006_023541b0(void) {
  Ov006_PrepareScriptScreen();
  return EventScript_Start(data_ov006_0236b1fc);
}

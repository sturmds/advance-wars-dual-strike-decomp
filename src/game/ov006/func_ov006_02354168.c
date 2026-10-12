#include "types.h"

void Ov006_PrepareScriptScreen(void);
int EventScript_Start(void*);
extern u8 data_ov006_0236b35c[];

int func_ov006_02354168(void) {
  Ov006_PrepareScriptScreen();
  return EventScript_Start(data_ov006_0236b35c);
}

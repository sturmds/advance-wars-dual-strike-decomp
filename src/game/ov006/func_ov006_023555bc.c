#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void func_ov006_02357734(void);
void func_ov006_02358fec(void);

void func_ov006_023555bc(void) {
  switch (data_ov006_0236d640->request) {
    case 2:
      func_ov006_02357734();
      break;
    case 6:
      func_ov006_02358fec();
  }
}

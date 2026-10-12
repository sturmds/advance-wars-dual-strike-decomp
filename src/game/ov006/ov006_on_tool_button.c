#include "types.h"

int func_ov006_02357694(void);
void Ov006_SetEntryPage(int page);

/* Tool buttons 96-100 (Ov006_AddToolButtons) pick entry pages 1, 2, 4, 3
 * and 0. */
void Ov006_OnToolButton(u8* buttonId) {
  int page = 1;

  if (func_ov006_02357694() == 0) {
    return;
  }
  switch (*buttonId) {
    case 96:
      page = 1;
      break;
    case 97:
      page = 2;
      break;
    case 98:
      page = 4;
      break;
    case 99:
      page = 3;
      break;
    case 100:
      page = 0;
      break;
  }
  Ov006_SetEntryPage(page);
}

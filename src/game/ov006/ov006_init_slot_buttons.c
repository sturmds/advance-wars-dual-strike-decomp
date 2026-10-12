#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_LoadSlotButtonGraphics(void);
void Ov006_AddSlotButtons(void);
extern Unk_ov006_0235f690* data_ov006_0236d640;

void Ov006_InitSlotButtons(void) {
  Ov006_LoadSlotButtonGraphics();
  Ov006_AddSlotButtons();
  data_ov006_0236d640->unk_39 = 32;
}

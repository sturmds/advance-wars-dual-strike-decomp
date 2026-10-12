#include "types.h"
#include "game/keypad.h"

void CampaignMap_Cancel(int unused);
void CampaignMap_Confirm(int unused);

/* Key handler of the mission buttons: A or Start confirms, B cancels. */
void CampaignMap_HandleKeys(void) {
  u16 keys = gKeypad->pressed;

  if (keys & (PAD_BUTTON_A | PAD_BUTTON_START)) {
    CampaignMap_Confirm(0);
  } else if (keys & PAD_BUTTON_B) {
    CampaignMap_Cancel(0);
  }
}

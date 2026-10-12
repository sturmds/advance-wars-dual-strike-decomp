#include "types.h"
#include "game/script_slot.h"

void func_020f5888(int tileX, int tileY);
void Sound_PlaySubSe(u32 soundId);

/* First callback of the service cost popup script: scrolls the view to the
 * serviced tile and sets the amount ServiceCostPopup_CountDown takes off per
 * frame: cost / 16, at least 1, or 50 from a cost of 1000. Then plays the
 * counting sound. */
void ServiceCostPopup_Begin(ServiceCostPopupSlot* slot) {
  func_020f5888(slot->tileX, slot->tileY);
  slot->unk_35 = 0;
  if (slot->cost >= 1000) {
    slot->step = 50;
  } else {
    slot->step = slot->cost >> 4;
    if (slot->step == 0) {
      slot->step = 1;
      slot->unk_35 = 16 - slot->cost;
      if ((s8)slot->unk_35 < 0) {
        slot->unk_35 = 16;
      }
    }
  }
  Sound_PlaySubSe(0x10005);
}

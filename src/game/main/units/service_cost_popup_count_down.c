#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void ScriptSlot_Stop(int slot);
void Sound_StopSubSe(int fadeFrames);
void Sound_PlaySe(u32 soundId);
void func_020d0478(void); /* destroys the task running script data_021664b8 */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* Script callback of the service cost popup: takes step off the shown cost
 * each frame, and off the current player's funds too when chargeFunds is 1. When
 * the cost reaches 0 the funds are set to fundsAfter (chargeFunds is 1), the task
 * of data_021664b8 is destroyed (visible set), the slot stops and sound
 * 0x10004 plays after the sub sound effect stops. */
void ServiceCostPopup_CountDown(ServiceCostPopupSlot* slot) {
  slot->cost -= slot->step;
  if (slot->cost > 0) {
    if (slot->chargeFunds == 1) {
      MAP_STATE->players[MAP_STATE->currentPlayer].funds -= slot->step;
    }
    return;
  }
  if (slot->chargeFunds == 1) {
    MAP_STATE->players[MAP_STATE->currentPlayer].funds = slot->fundsAfter;
  }
  if (slot->visible != 0) {
    func_020d0478();
  }
  ScriptSlot_Stop(*data_027c027c->scriptSlotCurrent);
  Sound_StopSubSe(0);
  Sound_PlaySe(0x10004);
}

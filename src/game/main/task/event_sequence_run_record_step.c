#include "types.h"
#include "game/event_script.h"

int EventChoice_GetCampaignSlot(void);
void Save_StoreProfile(int campaignSlot, int day);
int BattleSave_GetRecordId(int mode, int campaignSlot);
void func_020d8028(int arg);
void Save_PackAndWriteProgress(void);
void BattleSave_Delete(u8 recordId);

/* Event script handler. The record id in slot->unk_10 (set by
 * EventSequence_StartScript from BattleSave_GetRecordId) picks the work: 0 runs
 * Save_PackAndWriteProgress; 2 to 4 (campaign records) run Save_StoreProfile with day 0 for
 * the campaign slot, then BattleSave_Delete on that slot's record id; any other
 * id goes straight to BattleSave_Delete. What BattleSave_Delete does with the record
 * is not clear yet (it may load rather than save). */
void EventSequence_RunRecordStep(EventScriptSlot* slot) {
  int campaignSlot;

  func_020d8028(slot->unk_10);
  switch (slot->unk_10) {
    case 0:
      Save_PackAndWriteProgress();
      break;
    case 2:
    case 3:
    case 4:
      campaignSlot = EventChoice_GetCampaignSlot();
      Save_StoreProfile(campaignSlot, 0);
      BattleSave_Delete(BattleSave_GetRecordId(0, campaignSlot));
      break;
    default:
      BattleSave_Delete(slot->unk_10);
      break;
  }
}

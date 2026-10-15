#include "types.h"
#include "game/Unk_02080440.h"

BOOL Game_IsCampaign(void);
int EventChoice_GetCampaignSlot(void);
void Save_StoreProfile(int campaignSlot, int day);
int BattleSave_GetRecordId(int mode, int campaignSlot);
void func_020d8028(int arg);
void BattleSave_Save(int recordId, int flag);

/* Writes the current game to its save record: for a campaign game the record
 * for the campaign slot (EventChoice_GetCampaignSlot), otherwise the one for the game mode.
 * BattleSave_GetRecordId maps both to a record id from 1 to 6. */
void Save_SaveCurrentGame(void) {
  int campaignSlot;
  int recordId;

  if (Game_IsCampaign()) {
    campaignSlot = EventChoice_GetCampaignSlot();
    Save_StoreProfile(campaignSlot, MAP_STATE->day);
    recordId = BattleSave_GetRecordId(0, campaignSlot);
    func_020d8028(recordId);
    BattleSave_Save(recordId, 0);
  } else {
    recordId = BattleSave_GetRecordId(MAP_STATE->gameMode, 0);
    func_020d8028(recordId);
    BattleSave_Save(recordId, 0);
  }
}

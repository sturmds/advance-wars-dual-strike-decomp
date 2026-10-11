#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL Game_IsCampaign(void);
int func_020d82c8(void);
void func_02103ed8(int campaignSlot, int day);
int func_02019330(int mode, int campaignSlot);
void func_020d8028(int arg);
void BattleSave_Save(int recordId, int flag);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Writes the current game to its save record: for a campaign game the record
 * for the campaign slot (func_020d82c8), otherwise the one for the game mode.
 * func_02019330 maps both to a record id from 1 to 6. */
void Save_SaveCurrentGame(void)
{
    int campaignSlot;
    int recordId;

    if (Game_IsCampaign()) {
        campaignSlot = func_020d82c8();
        func_02103ed8(campaignSlot, STATE->day);
        recordId = func_02019330(0, campaignSlot);
        func_020d8028(recordId);
        BattleSave_Save(recordId, 0);
    } else {
        recordId = func_02019330(STATE->gameMode, 0);
        func_020d8028(recordId);
        BattleSave_Save(recordId, 0);
    }
}

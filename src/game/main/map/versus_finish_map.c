#include "types.h"
#include "game/screen_other_map.h"

int BattleSave_GetRecordId(int mode, int campaignSlot);
void BattleRecordTask_Start(int recordId, void (*callback)(void));
void Versus_StartVsCpuOneMap(void);
void Versus_StartVsCpuTwoMaps(void);
void GameFlagsMid_Restore(void);
void func_020e8c28(void);
void func_0210372c(void);

extern u32 gLinkPlay;

/* End of a Versus map. Offline, starts the BattleRecordTask_Start task for this mode's
 * record, with a callback that restarts one-map or two-map Versus. With
 * gLinkPlay set, runs the three calls below instead. */
void Versus_FinishMap(void)
{
    if (gLinkPlay == 0) {
        if (data_027c027c->unk_48[data_027c0284]->twoMaps == 0) {
            BattleRecordTask_Start(BattleSave_GetRecordId(data_027c027c->unk_48[data_027c0284]->gameMode, 0), Versus_StartVsCpuOneMap);
        } else {
            BattleRecordTask_Start(BattleSave_GetRecordId(data_027c027c->unk_48[data_027c0284]->gameMode, 0), Versus_StartVsCpuTwoMaps);
        }
    } else {
        GameFlagsMid_Restore();
        func_020e8c28();
        func_0210372c();
    }
}

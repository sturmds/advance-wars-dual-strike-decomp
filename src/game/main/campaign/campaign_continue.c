#include "types.h"
#include "game/Unk_02290118.h"

void CampaignMap_ResetState(void);
int BattleSave_GetRecordId(int a, int b);
void BattleRecordTask_Start(int a, void (*callback)(void));
void func_021037ac(void);

/* Resets the campaign map state, then starts the BattleRecordTask_Start task with
 * func_021037ac, which loads the saved profiles, as its callback. */
void Campaign_Continue(void) {
  CampaignMap_ResetState();
  BattleRecordTask_Start(BattleSave_GetRecordId(0, data_02290118.unk_00),
                         func_021037ac);
}

#include "types.h"
#include "game/Unk_02290118.h"

void CampaignMap_ResetState(void);
int func_02019330(int a, int b);
void func_020d8294(int a, void (*callback)(void));
void func_021037ac(void);

/* Resets the campaign map state, then starts the func_020d8294 task with
 * func_021037ac, which loads the saved profiles, as its callback. */
void Campaign_Continue(void)
{
    CampaignMap_ResetState();
    func_020d8294(func_02019330(0, data_02290118.unk_00), func_021037ac);
}

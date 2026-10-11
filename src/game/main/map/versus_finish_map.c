#include "types.h"
#include "game/screen_other_map.h"

int BattleSave_GetRecordId(int mode, int campaignSlot);
void func_020d8294(int recordId, void (*callback)(void));
void Versus_StartVsCpuOneMap(void);
void Versus_StartVsCpuTwoMaps(void);
void func_02102304(void);
void func_020e8c28(void);
void func_0210372c(void);

extern u32 data_0229eaec;

/* End of a Versus map. Offline, starts the func_020d8294 task for this mode's
 * record, with a callback that restarts one-map or two-map Versus. With
 * data_0229eaec set, runs the three calls below instead. */
void Versus_FinishMap(void)
{
    if (data_0229eaec == 0) {
        if (data_027c027c->unk_48[data_027c0284]->twoMaps == 0) {
            func_020d8294(BattleSave_GetRecordId(data_027c027c->unk_48[data_027c0284]->gameMode, 0), Versus_StartVsCpuOneMap);
        } else {
            func_020d8294(BattleSave_GetRecordId(data_027c027c->unk_48[data_027c0284]->gameMode, 0), Versus_StartVsCpuTwoMaps);
        }
    } else {
        func_02102304();
        func_020e8c28();
        func_0210372c();
    }
}

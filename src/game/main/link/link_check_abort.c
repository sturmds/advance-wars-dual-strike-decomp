#include "types.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c *data_0215fe8c;

u16 Wireless_GetChildAids(void);
int func_0207a184(void);
int Wireless_GetState(void);
int func_02077f20(void);
void Link_Stop(void);
void SpriteTask_DestroyAllFirstScreen(void);
void func_0207d934(void);

/* Flags an error when the stored value (unk_08) no longer matches Wireless_GetChildAids or
 * func_0207a184 returns 1. On an error, or when Wireless_GetState returns 1, stops link mode and resets. */
void Link_CheckAbort(void)
{
    Unk_0215fe8c *link;

    if (data_0215fe8c->unk_0c != 0 && func_02077f20() == 0) {
        link = data_0215fe8c;
        if (link->unk_08 != Wireless_GetChildAids()) {
            link->error = 1;
        }
    }
    if (data_0215fe8c->unk_0c != 0 && func_0207a184() == 1) {
        data_0215fe8c->error = 1;
    }
    if (data_0215fe8c->error != 1 && Wireless_GetState() != 1) {
        return;
    }
    Link_Stop();
    SpriteTask_DestroyAllFirstScreen();
    func_0207d934();
}

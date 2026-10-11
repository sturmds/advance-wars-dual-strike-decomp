#include "types.h"
#include "game/Unk_02160ac0.h"

void Sound_PlaySe(u32 soundId);
void func_0208aca0(void);
void CombatModeMenu_DrawFundsAndPrices(void);

#define SE_CANCEL 0x10009

/* Takes back the last unit bought in the Combat mode shop and refunds its
 * price (the reverse of CombatModeMenu_BuyUnit). Button 14 goes away
 * (func_0208aca0) once no unit is left. */
void CombatModeMenu_SellLastUnit(void)
{
    if (data_02160ac0->unk_18 == 0) {
        return;
    }
    data_02160ac0->unk_18--;
    switch (data_02160ac0->cells[data_02160ac0->unk_18].tile) {
    case 1:
        data_02160ac0->unk_14 += 1000;
        break;
    case 3:
        data_02160ac0->unk_14 += 3000;
        break;
    case 4:
        data_02160ac0->unk_14 += 5000;
        break;
    case 5:
        data_02160ac0->unk_14 += 6000;
        break;
    }
    CombatModeMenu_DrawFundsAndPrices();
    if (data_02160ac0->unk_18 == 0) {
        func_0208aca0();
    }
    Sound_PlaySe(SE_CANCEL);
}

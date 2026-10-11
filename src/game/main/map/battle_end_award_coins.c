#include "types.h"
#include "game/Unk_022a7a9c.h"

void CallbackSlots_Clear(void);
void PlayRecords_AddCoins(u32 amount);
void func_01ff9c28(void (*callback)(void));
void func_01ff9c48(void (*callback)(void));
void func_020b9864(void);
void func_020b9890(void);
int BattleEnd_HasCoinReward(void);
void MainLoop_DefaultVBlank(void);
void func_02000fa4(void);

/* Puts the main loop's callbacks back and clears the callback slots. When
 * BattleEnd_HasCoinReward allows it, adds the difference of data_022a7a9c.unk_14 and
 * unk_18 to the coin total; otherwise runs func_020b9864. */
void BattleEnd_AwardCoins(void)
{
    func_020b9890();
    func_01ff9c28(MainLoop_DefaultVBlank);
    func_01ff9c48(func_02000fa4);
    CallbackSlots_Clear();
    if (!BattleEnd_HasCoinReward()) {
        func_020b9864();
    } else {
        PlayRecords_AddCoins(data_022a7a9c.unk_14 - data_022a7a9c.unk_18);
    }
}

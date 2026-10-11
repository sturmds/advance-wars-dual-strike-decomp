#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 data_0229d8c4;
extern u32 data_022a6b38;

void func_020cafe4(void);
void func_020b9c3c(void);
void func_020b98bc(void);
void func_020c7104(void);
void func_020db3c0(void);
BOOL Map_IsCoSelectPhase(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Clears the map state's view and menu fields before a battle starts and sets
 * day 1, player 0. On a map where a player picks a CO first, the day starts at
 * 0. The name is a guess from the callers (battle setup hooks). */
void Battle_ResetTurnState(void)
{
    STATE->unk_28 = 0;
    STATE->unk_2a = 0;
    STATE->unk_2c = 0;
    STATE->unk_2e = 0;
    func_020cafe4();
    STATE->unk_10 = 0;
    STATE->unk_14 = 0;
    func_020b9c3c();
    func_020b98bc();
    data_0229d8c4 = 0;
    data_022a6b38 = 1;
    STATE->unk_18 = 4;
    STATE->unk_1c = 0;
    STATE->currentPlayer = 0;
    STATE->day = 1;
    func_020c7104();
    func_020db3c0();
    if (Map_IsCoSelectPhase()) {
        STATE->day = 0;
    }
}

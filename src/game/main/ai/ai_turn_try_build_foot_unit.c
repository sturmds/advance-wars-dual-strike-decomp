#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

typedef struct Unk_0229e44c {
    u8 pad_00[3];
    u8 unk_03_0 : 1;
    u8 unk_03_1 : 1;
} Unk_0229e44c;

extern Unk_0229e44c data_0229e44c[];
extern s8 data_0229e5c0[][12];

#define STATE (data_027c027c->unk_48[data_027c0284])

int Rng_Next(void);
s32 HwMod32(s32 numer, s32 denom);
int Unit_GetValuePerHp(int playerIndex, int unitType);
int func_02096378(void);
int AiSite_PickNearestToPropertyAnyGroup(int arg);
int AiSite_PickFirst(int arg);
int func_0209f980(int arg);
u32 func_02091b24(void);
void func_020b81bc(void);
void func_0209316c(int index);

/* End-actions step 6. Tries to start building a foot unit if funds cover an
 * Infantry. Takes the first factory that func_02096378, AiSite_PickNearestToPropertyAnyGroup(1) or
 * AiSite_PickFirst(1) return (-1: none). A factory that already has bit 0 of
 * byte 3 in its data_0229e44c record set is skipped when unk_08 reaches the
 * profile's unk_08[0] + unk_08[1]. The unit is a Mech when a roll of 0 to 99
 * is under func_02091b24() and funds cover 20 times its value per HP, else an
 * Infantry. The name and the meaning of the flags are guesses. */
BOOL AiTurn_TryBuildFootUnit(void)
{
    int index;
    u32 roll;
    Unk_0229e44c *record;

    if ((u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_INFANTRY) * 10) > STATE->players[STATE->currentPlayer].funds) {
        return 0;
    }
    index = func_02096378();
    if (index == -1) {
        index = AiSite_PickNearestToPropertyAnyGroup(1);
    }
    if (index == -1) {
        index = AiSite_PickFirst(1);
    }
    if (index == -1) {
        return 0;
    }
    record = &data_0229e44c[data_0229e5c0[index][0]];
    if (record->unk_03_0 == 0) {
        record->unk_03_0 = 1;
    } else {
        data_021619c0->unk_08 = func_0209f980(1);
        if (data_021619c0->unk_08 >= gAiProfile->unk_08[0] + gAiProfile->unk_08[1]) {
            return 0;
        }
    }
    roll = HwMod32(Rng_Next(), 100);
    if (roll < func_02091b24() && (u32)STATE->players[STATE->currentPlayer].funds >= (u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_MECH) * 20)) {
        gUnitAction.target = UNIT_MECH;
    } else {
        gUnitAction.target = UNIT_INFANTRY;
    }
    func_020b81bc();
    func_0209316c(index);
    return 1;
}

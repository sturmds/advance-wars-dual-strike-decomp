#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

BOOL Player_IsPowerActive(int player);
int SuperPowerCost(int player, int slot);
BOOL PowerMeter_IsShown(int arg);
s32 HwDiv32(s32 numerator, s32 denominator);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_02080440_48 data_02183b08;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Adds meter charge to one CO of a player (slot 0 front, 1 back), capped at
 * the Super CO Power cost. Skipped when CO effects are off or a power is on.
 * Star Power adds 10%. */
void Player_AddPowerCharge(int player, int slot, int amount)
{
    if (STATE->unk_5c == 0) {
        return;
    }
    if (Player_IsPowerActive(player)) {
        return;
    }
    if (!PowerMeter_IsShown(STATE == &data_02183b08 ? MAP_STATE_FIRST : MAP_STATE_SECOND)) {
        return;
    }
    if (CheckSkillsEquipped(STATE->players[player].skills[slot], SKILL_RANK_STAR_POWER)) {
        amount = HwDiv32(amount * 110, 100);
    }
    if (amount + (&STATE->players[player].front)[slot].charge > SuperPowerCost(player, slot)) {
        (&STATE->players[player].front)[slot].charge = SuperPowerCost(player, slot);
    } else {
        (&STATE->players[player].front)[slot].charge = amount + (&STATE->players[player].front)[slot].charge;
    }
}

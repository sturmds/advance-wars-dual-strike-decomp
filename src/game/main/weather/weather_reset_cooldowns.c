#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define WEATHER (data_027c027c->unk_5c[data_027c0284])

void func_020d2190(void);

/* Clears every player's weather days and sets the three bytes unk_00 to unk_02 to 6
 * minus the number of players that have a controller. */
void Weather_ResetCooldowns(void)
{
    int player;
    int cooldown2;
    int cooldown1;
    int cooldown0;
    cooldown2 = 6;
    cooldown1 = 6;
    cooldown0 = 6;

    for (player = 1; player <= 4; player++) {
        if (STATE->players[player].controller != PLAYER_CONTROLLER_NONE) {
            cooldown1--;
            cooldown2--;
            cooldown0--;
        }
        WEATHER->weatherDays[player] = 0;
    }
    WEATHER->unk_02 = cooldown2;
    WEATHER->unk_01 = cooldown1;
    WEATHER->unk_00 = cooldown0;
    func_020d2190();
}

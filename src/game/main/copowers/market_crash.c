#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

s32 func_02002fa4(s32 numerator, s32 denominator);
void func_0200b76c(int id);
BOOL func_020c237c(u8 player);
void func_020e2670(int priority);
int SuperPowerCost(int player, int slot);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Byte view of frontCo/backCo. The game reads the CO id here with ldrb
 * and indexes by slot; a plain u8 cast of the word does not compile the
 * same way. */
typedef struct CoSlotByte {
    u8 id;
    u8 pad_01[3];
} CoSlotByte;

/* Sasha's CO Power: each CO (front and back) of every enemy still in the
 * game loses meter charge equal to percent of its Super cost, where
 * percent is the current player's funds / 500, capped at 100. */
void MarketCrash(int priority)
{
    int percent;
    int hundred; /* needed to match: the divisor is kept on the stack */
    int charge;
    int player;
    int slot;

    func_0200b76c(0xe3);
    func_020e2670(priority);
    percent = func_02002fa4(data_027c027c->unk_48[data_027c0284]->players[data_027c027c->unk_48[data_027c0284]->currentPlayer].funds, 500);
    if (percent > 100) {
        percent = 100;
    }
    hundred = 100;
    for (player = 1; player < 5; player++) {
        if (!func_020c237c(player)) {
            continue;
        }
        if (data_027c027c->unk_48[data_027c0284]->players[data_027c027c->unk_48[data_027c0284]->currentPlayer].team ==
            data_027c027c->unk_48[data_027c0284]->players[player].team) {
            continue;
        }
        for (slot = 0; slot < 2; slot++) {
            Player *p = &data_027c027c->unk_48[data_027c0284]->players[player];

            if (((CoSlotByte *)&p->frontCo)[slot].id & PLAYER_CO_ID_MASK) {
                int cost;

                charge = PLAYER_CO_STATUS((&data_027c027c->unk_48[data_027c0284]->players[player].frontCo)[slot])->charge;
                cost = SuperPowerCost(player, slot);
                cost = func_02002fa4(cost * percent, hundred);
                if (charge < cost) {
                    charge = 0;
                } else {
                    charge -= cost;
                }
                PLAYER_CO_STATUS((&data_027c027c->unk_48[data_027c0284]->players[player].frontCo)[slot])->charge = charge;
            }
        }
    }
}

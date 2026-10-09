#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02290718.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020c76c4(MapUnit *unit); /* puts the unit on the other map */

void UnitAction_MoveToOtherMap(void)
{
    if (data_027c027c->unk_48[data_027c0284]->players[data_027c027c->unk_48[data_027c0284]->currentPlayer].controller == PLAYER_CONTROLLER_HUMAN) {
        if (data_02290718.unitsSentToOtherMap < 999999) {
            data_02290718.unitsSentToOtherMap++;
        }
    }
    func_020c76c4(data_027c027c->unk_48[data_027c0284]->actingUnit);
}

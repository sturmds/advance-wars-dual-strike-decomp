#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02290718.h"
#include "game/player.h"
#include "game/script_slot.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern const ScriptCommand data_02166450[];

ScriptSlot *ScriptSlot_Start(const ScriptCommand *script, int arg);

void func_020ddc2c(void)
{
    if (data_027c027c->unk_48[data_027c0284]->players[data_027c027c->unk_48[data_027c0284]->currentPlayer].controller == PLAYER_CONTROLLER_HUMAN) {
        if (data_02290718.unk_3ec < 999999) {
            data_02290718.unk_3ec++;
        }
    }
    ScriptSlot_Start(data_02166450, 0)->unk_20 = 1;
}

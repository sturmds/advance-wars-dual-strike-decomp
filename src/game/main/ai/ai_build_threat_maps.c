#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player); /* TRUE: the player is still in the game */
void func_0209ca48(void);       /* clears MapTiles.unk_3250 and unk_3758 */
void func_0209cacc(u8 player);  /* adds one enemy player's units to the maps */
void func_0209d4e0(void);
void func_0209d610(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Fills the per-tile maps the AI checks before moving (MapTiles.unk_3250
 * and unk_3758) from every enemy player still in the game. Runs once per
 * queued unit: AiTurn_RunQueue clears threatMapsBuilt. */
void Ai_BuildThreatMaps(void)
{
    u16 savedUnk1e;
    int player;

    savedUnk1e = STATE->unk_1e;
    if (AI_TARGET_FLAGS->threatMapsBuilt) {
        return;
    }
    AI_TARGET_FLAGS->threatMapsBuilt = TRUE;
    func_0209ca48();
    func_0209d4e0();
    func_0209d610();
    for (player = 1; player < 5; player++) {
        if (Player_IsInGame(player) && STATE->players[STATE->currentPlayer].team != STATE->players[player].team) {
            func_0209cacc(player);
        }
    }
    STATE->unk_1e = savedUnk1e;
}

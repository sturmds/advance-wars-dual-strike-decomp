#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
void Ai_ClearThreatMaps(void);
void func_01ffb200(void *tileMap);
void func_01ffb3a0(int value);
void func_0209b458(void);
void func_0209b918(u8 player, int arg);
void Ai_AddObjectThreat(void);
void Ai_MarkObjectTileThreat(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Builds the AI threat maps like Ai_BuildThreatMaps for every enemy player
 * still in the game (func_0209b918 per player, arg passed through), with
 * the range map switched to unk_2d48 filled with 0 meanwhile and to unk_2840
 * after. Unless the current player's AI behaviour is Defense it then calls
 * func_0209b458, and it leaves unk_2d48 filled with -1. */
void func_0209b2b0(int arg)
{
    u16 savedUnk1e;
    int player;

    savedUnk1e = STATE->unk_1e;
    func_01ffb200(STATE->mapTiles->unk_2d48);
    func_01ffb3a0(0);
    func_01ffb200(STATE->mapTiles->unk_2840);
    Ai_ClearThreatMaps();
    Ai_AddObjectThreat();
    Ai_MarkObjectTileThreat();
    for (player = 1; player < 5; player++) {
        if (Player_IsInGame(player) && STATE->players[STATE->currentPlayer].team != STATE->players[player].team) {
            func_0209b918(player, arg);
        }
    }
    STATE->unk_1e = savedUnk1e;
    if (STATE->players[STATE->currentPlayer].aiBehaviour != AI_BEHAVIOUR_DEFENSE) {
        func_0209b458();
    }
    func_01ffb200(STATE->mapTiles->unk_2d48);
    func_01ffb3a0(-1);
    func_01ffb200(STATE->mapTiles->unk_2840);
}

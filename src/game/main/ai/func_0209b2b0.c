#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
void Ai_ClearThreatMaps(void);
void Ai_ClearThreatInAntiAirCover(void);
void AiBlackBomb_AddEnemyThreat(u8 player, int arg);
void Ai_AddObjectThreat(void);
void Ai_MarkObjectTileThreat(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Builds the AI threat maps like Ai_BuildThreatMaps for every enemy player
 * still in the game (AiBlackBomb_AddEnemyThreat per player, arg passed through), with
 * the range map switched to rangeScratch filled with 0 meanwhile and to rangeMapTiles
 * after. Unless the current player's AI behaviour is Defense it then calls
 * Ai_ClearThreatInAntiAirCover, and it leaves rangeScratch filled with -1. */
void func_0209b2b0(int arg) {
  u16 savedUnk1e;
  int player;

  savedUnk1e = STATE->unk_1e;
  RangeMap_SetBuffer(STATE->mapTiles->rangeScratch);
  RangeMap_Fill(0);
  RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
  Ai_ClearThreatMaps();
  Ai_AddObjectThreat();
  Ai_MarkObjectTileThreat();
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) && STATE->players[STATE->currentPlayer].team !=
                                       STATE->players[player].team) {
      AiBlackBomb_AddEnemyThreat(player, arg);
    }
  }
  STATE->unk_1e = savedUnk1e;
  if (STATE->players[STATE->currentPlayer].aiBehaviour !=
      AI_BEHAVIOUR_DEFENSE) {
    Ai_ClearThreatInAntiAirCover();
  }
  RangeMap_SetBuffer(STATE->mapTiles->rangeScratch);
  RangeMap_Fill(-1);
  RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
}

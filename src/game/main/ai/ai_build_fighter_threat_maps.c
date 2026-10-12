#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
BOOL Player_IsInGame(u8 player);
void Ai_ClearThreatMaps(void);
void Ai_AddObjectThreat(void);
void Ai_MarkObjectTileThreat(void);
void Ai_AddEnemyThreatToFighter(u8 player);
#define STATE (data_027c027c->unk_48[data_027c0284])

void Ai_BuildFighterThreatMaps(void) {
  u16 savedUnk1e;
  int player;

  savedUnk1e = STATE->unk_1e;
  Ai_ClearThreatMaps();
  Ai_AddObjectThreat();
  Ai_MarkObjectTileThreat();
  for (player = 1; player < 5; player++) {
    if (Player_IsInGame(player) && STATE->players[STATE->currentPlayer].team !=
                                       STATE->players[player].team) {
      Ai_AddEnemyThreatToFighter(player);
    }
  }
  STATE->unk_1e = savedUnk1e;
}

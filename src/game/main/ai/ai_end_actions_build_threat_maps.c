#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
void Ai_ClearThreatMaps(void);
void Ai_AddObjectThreat(void);
void Ai_MarkObjectTileThreat(void);
void AiEndActions_AddEnemyThreat(u8 player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Counterpart of Ai_BuildThreatMaps for the end-of-turn phase: same clearing and
 * object threat, but each enemy player is added by AiEndActions_AddEnemyThreat,
 * which rates threat against an Anti-Air instead of the acting unit. The name
 * is a guess from the caller. */
void AiEndActions_BuildThreatMaps(void) {
  u16 savedUnk1e;
  int player;

  savedUnk1e = STATE->unk_1e;
  Ai_ClearThreatMaps();
  Ai_AddObjectThreat();
  Ai_MarkObjectTileThreat();
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) && STATE->players[STATE->currentPlayer].team !=
                                       STATE->players[player].team) {
      AiEndActions_AddEnemyThreat(player);
    }
  }
  STATE->unk_1e = savedUnk1e;
}

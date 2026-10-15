#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai/ai_threat.h"

BOOL Player_IsInGame(u8 player);

void func_0209c1f8(void) {
  u16 savedUnk1e;
  int player;

  savedUnk1e = MAP_STATE->unk_1e;
  Ai_ClearThreatMaps();
  Ai_AddObjectThreat();
  Ai_MarkObjectTileThreat();
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
      Ai_AddIndirectEnemyThreat(player);
    }
  }
  MAP_STATE->unk_1e = savedUnk1e;
}

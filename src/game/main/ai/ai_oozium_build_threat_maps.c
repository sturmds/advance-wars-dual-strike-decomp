#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
void Ai_ClearThreatMaps(void);
void Ai_AddOoziumThreat(
    MapUnit*
        unit); /* an Oozium in sight sets threatSum to 255 on the tiles next to it */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Clears the threat maps and adds the threat of each unit of every enemy
 * player still in the game. Unlike Ai_BuildThreatMaps it does not call
 * Ai_AddObjectThreat and Ai_MarkObjectTileThreat. Units use slots 1 to 50 of the player's
 * block. */
void AiOozium_BuildThreatMaps(void) {
  int player;
  int slot;
  MapUnit* unit;

  Ai_ClearThreatMaps();
  for (player = 1; player < 5; player++) {
    if (Player_IsInGame(player) && STATE->players[STATE->currentPlayer].team !=
                                       STATE->players[player].team) {
      for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
           slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
        unit = &STATE->units[slot];
        if (unit->type != 0) {
          Ai_AddOoziumThreat(unit);
        }
      }
    }
  }
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);

/* Clears transportWanted and two unit bits of unknown meaning (unk_0c_5, unk_08_7)
 * on the units of every player still in the game. Slots 1 to 50 of each
 * player's 64 are visited; the name is a guess. */
void AiTurn_ClearUnitMarks(void) {
  int player;
  int slot;
  int first;

  for (player = 1; player < 5; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    first = Ov000_gPlayerFirstUnitSlot[player];
    for (slot = first + 1; slot < first + 51; slot++) {
      MapUnit* unit = &data_027c027c->unk_48[data_027c0284]->units[slot];

      if (unit->type != UNIT_NONE) {
        unit->transportWanted = 0;
        unit->unk_0c_5 = 0;
        unit->unk_08_7 = 0;
      }
    }
  }
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Copies each player slot's army colour (Player.armyColour) into
 * gArmyColours. Map_LoadArmyColours reads the map header instead. */
void Map_LoadArmyColoursFromPlayers(void) {
  Unk_02080440* game = data_027c027c;
  int index = data_027c0284;
  int slot = 0;
  int offset = 0;
  u8* dst = gArmyColours;

  /* Needed to match: a do-while with a byte offset into players. */
  do {
    *dst++ =
        ((Player*)((u8*)game->unk_48[index]->players + offset))->armyColour;
    offset += sizeof(Player);
    slot++;
  } while (slot < ARMY_SLOT_COUNT);
}

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Tile_HasEnemyUnit(int x, int y, int arg2);
int UnitType_BestBaseDamage(int unitType, int targetType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Path search callback: TRUE when an enemy unit stands on tile (x, y) and
 * unitType can damage it (chart damage above 1). */
int Ai_CanDamageEnemyAtTile(int x, int y, int unitType) {
  if (!Tile_HasEnemyUnit(x, y, 0)) {
    return FALSE;
  }
  return UnitType_BestBaseDamage(
             unitType,
             STATE
                 ->units[STATE->mapTiles
                             ->unitAt[x + STATE->mapTiles->rowStart[y]]]
                 .type) > 1;
}

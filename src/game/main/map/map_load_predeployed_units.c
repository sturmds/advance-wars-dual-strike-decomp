#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/predeployed_unit.h"

BOOL Campaign_IsHardMode(void);
void Map_PlacePredeployedUnits(const PredeployedUnit* row);

/* Places the current map's pre-deployed units: MapHeader.units, or
 * hardUnits when Campaign_IsHardMode returns nonzero. Maps without a list are
 * skipped. */
void Map_LoadPredeployedUnits(void) {
  int hard = 0;
  const PredeployedUnit* units;

  if (Campaign_IsHardMode()) {
    hard = 1;
  }
  /* needed to match: units and hardUnits are read as a two-entry array */
  units =
      (&Ov000_gMapHeaders[data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId]
            .units)[hard];
  if (units != NULL) {
    Map_PlacePredeployedUnits(units);
  }
}

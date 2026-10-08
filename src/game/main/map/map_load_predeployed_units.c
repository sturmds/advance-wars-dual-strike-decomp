#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/predeployed_unit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL func_021039e4(void);
void Map_PlacePredeployedUnits(const PredeployedUnit *row);

/* Places the current map's pre-deployed units: MapHeader.units, or
 * hardUnits when func_021039e4 returns nonzero. Maps without a list are
 * skipped. */
void Map_LoadPredeployedUnits(void)
{
    int hard = 0;
    const u8 *units;

    if (func_021039e4()) {
        hard = 1;
    }
    /* needed to match: units and hardUnits are read as a two-entry array */
    units = (&Ov000_gMapHeaders[data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId].units)[hard];
    if (units != NULL) {
        Map_PlacePredeployedUnits((const PredeployedUnit *)units);
    }
}

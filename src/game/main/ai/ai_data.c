#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"

AiTurnState* gAiTurnState = &data_0229e178;
AiProfile* gAiProfile = &data_0229e2e4;
AiBuildState* data_021619c0 = &data_0229e184;
/* per unit type; nonzero: ground units a Lander carries, without Piperunner */
u8 gUnitFitsLander[] = {
#include "data/main/gUnitFitsLander.inc.c"
};
/* per unit type: 1 ground, 2 air, 3 sea */
u8 gUnitSupplyKind[] = {
#include "data/main/gUnitSupplyKind.inc.c"
};
/* per terrain id: 1 for properties */
u8 gTerrainIsProperty[] = {
#include "data/main/gTerrainIsProperty.inc.c"
};
/* per terrain id: 1 base, 2 airport, 3 port, else 0 */
u8 gTerrainFactoryKind[] = {
#include "data/main/gTerrainFactoryKind.inc.c"
};
/* per terrain id: kind of unit it supplies, 1 ground (city, HQ, base), 2 air, 3 sea */
u8 gTerrainSupplyKind[] = {
#include "data/main/gTerrainSupplyKind.inc.c"
};

#include "types.h"
#include "game/event_script.h"
#include "game/map_script.h"

void MapObjects_ClearKind2Variants(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void OtherMapObjects_ClearKind2Variants(void);
void Player1Team_HasPlayerInGame(void);
extern EventTrigger data_ov001_02369c10[];
extern EventScriptCommand data_ov001_02369c58[];
extern EventScriptCommand data_ov001_0236a478[];
extern EventScriptCommand data_ov001_0236a538[];
extern EventScriptCommand data_ov001_0236a948[];
extern EventScriptCommand data_ov001_0236ad18[];
void func_ov001_02351804(void);
void func_ov001_02351a4c(void);
void func_ov001_02351af0(void);
extern u8 data_02168140[];
extern EventScriptCommand Ov001_gDefeatOtherTeams2Script[];
extern EventTrigger data_ov001_02369bf0[];
extern EventScriptCommand data_ov001_02369d18[];
extern EventScriptCommand data_ov001_02369da8[];
extern EventScriptCommand data_ov001_0236a618[];
extern EventScriptCommand data_ov001_0236a748[];
extern EventTrigger data_ov001_02369cb8[];

EventTrigger data_ov001_02369bb4[] = {
#include "data/ov001/data_ov001_02369bb4.inc.c"
};
EventTrigger data_ov001_02369bac[] = {
#include "data/ov001/data_ov001_02369bac.inc.c"
};
EventTrigger data_ov001_02369bbc[] = {
#include "data/ov001/data_ov001_02369bbc.inc.c"
};
/* Map 0x0ff, a second header with the same name, points here too. */
MapScript Ov001_gMapScript_0f5 = {data_ov001_02369c10,
                                  data_ov001_02369bf0,
                                  data_ov001_02369bb4,
                                  data_ov001_02369bbc,
                                  data_ov001_02369bac,
                                  data_ov001_02369cb8,
                                  NULL};
EventTrigger data_ov001_02369bf0[] = {
#include "data/ov001/data_ov001_02369bf0.inc.c"
};
EventTrigger data_ov001_02369c10[] = {
#include "data/ov001/data_ov001_02369c10.inc.c"
};
EventScriptCommand data_ov001_02369c58[] = {
#include "data/ov001/data_ov001_02369c58.inc.c"
};
EventTrigger data_ov001_02369cb8[] = {
#include "data/ov001/data_ov001_02369cb8.inc.c"
};
EventScriptCommand data_ov001_02369d18[] = {
#include "data/ov001/data_ov001_02369d18.inc.c"
};
EventScriptCommand data_ov001_02369da8[] = {
#include "data/ov001/data_ov001_02369da8.inc.c"
};
EventScriptCommand data_ov001_0236a338[] = {
#include "data/ov001/data_ov001_0236a338.inc.c"
};
EventScriptCommand data_ov001_0236a018[] = {
#include "data/ov001/data_ov001_0236a018.inc.c"
};
EventScriptCommand data_ov001_02369f78[] = {
#include "data/ov001/data_ov001_02369f78.inc.c"
};
EventScriptCommand data_ov001_02369ed8[] = {
#include "data/ov001/data_ov001_02369ed8.inc.c"
};
EventScriptCommand data_ov001_0236a0b8[] = {
#include "data/ov001/data_ov001_0236a0b8.inc.c"
};
EventScriptCommand data_ov001_0236a158[] = {
#include "data/ov001/data_ov001_0236a158.inc.c"
};
EventScriptCommand data_ov001_0236a1f8[] = {
#include "data/ov001/data_ov001_0236a1f8.inc.c"
};
EventScriptCommand data_ov001_0236a298[] = {
#include "data/ov001/data_ov001_0236a298.inc.c"
};
EventScriptCommand data_ov001_0236a3d8[] = {
#include "data/ov001/data_ov001_0236a3d8.inc.c"
};
EventScriptCommand data_ov001_02369e38[] = {
#include "data/ov001/data_ov001_02369e38.inc.c"
};
EventScriptCommand data_ov001_0236a478[] = {
#include "data/ov001/data_ov001_0236a478.inc.c"
};
EventScriptCommand data_ov001_0236a538[] = {
#include "data/ov001/data_ov001_0236a538.inc.c"
};
EventScriptCommand data_ov001_0236a618[] = {
#include "data/ov001/data_ov001_0236a618.inc.c"
};
EventScriptCommand data_ov001_0236a748[] = {
#include "data/ov001/data_ov001_0236a748.inc.c"
};
EventScriptCommand data_ov001_0236a948[] = {
#include "data/ov001/data_ov001_0236a948.inc.c"
};
EventScriptCommand data_ov001_0236ad18[] = {
#include "data/ov001/data_ov001_0236ad18.inc.c"
};

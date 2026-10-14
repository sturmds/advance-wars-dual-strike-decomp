#include "types.h"
#include "game/event_script.h"
#include "game/map_script.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Player1Team_HasPlayerInGame(void);
extern EventTrigger data_ov001_02371838[];
extern EventScriptCommand data_ov001_02371870[];
extern EventScriptCommand data_ov001_02371918[];
extern EventScriptCommand data_ov001_02371a58[];
extern EventScriptCommand data_ov001_02371c78[];
extern EventScriptCommand data_ov001_02371ed8[];
extern EventTrigger data_ov001_023718b0[];
extern EventScriptCommand data_ov001_023719b8[];
extern EventScriptCommand data_ov001_02371b68[];
extern EventScriptCommand data_ov001_02371d98[];

EventTrigger data_ov001_023717dc[] = {
#include "data/ov001/data_ov001_023717dc.inc.c"
};
EventTrigger data_ov001_023717e4[] = {
#include "data/ov001/data_ov001_023717e4.inc.c"
};
EventTrigger data_ov001_023717ec[] = {
#include "data/ov001/data_ov001_023717ec.inc.c"
};
EventTrigger data_ov001_023717f4[] = {
#include "data/ov001/data_ov001_023717f4.inc.c"
};
MapScript data_ov001_023717fc = {data_ov001_02371838,
                                 data_ov001_023717dc,
                                 data_ov001_023717f4,
                                 data_ov001_023717ec,
                                 data_ov001_023717e4,
                                 data_ov001_023718b0,
                                 NULL};
EventScriptCommand data_ov001_02371818[] = {
#include "data/ov001/data_ov001_02371818.inc.c"
};
EventTrigger data_ov001_02371838[] = {
#include "data/ov001/data_ov001_02371838.inc.c"
};
EventScriptCommand data_ov001_02371870[] = {
#include "data/ov001/data_ov001_02371870.inc.c"
};
EventTrigger data_ov001_023718b0[] = {
#include "data/ov001/data_ov001_023718b0.inc.c"
};
EventScriptCommand data_ov001_02371918[] = {
#include "data/ov001/data_ov001_02371918.inc.c"
};
EventScriptCommand data_ov001_023719b8[] = {
#include "data/ov001/data_ov001_023719b8.inc.c"
};
EventScriptCommand data_ov001_02371a58[] = {
#include "data/ov001/data_ov001_02371a58.inc.c"
};
EventScriptCommand data_ov001_02371b68[] = {
#include "data/ov001/data_ov001_02371b68.inc.c"
};
EventScriptCommand data_ov001_02371c78[] = {
#include "data/ov001/data_ov001_02371c78.inc.c"
};
EventScriptCommand data_ov001_02371d98[] = {
#include "data/ov001/data_ov001_02371d98.inc.c"
};
EventScriptCommand data_ov001_02371ed8[] = {
#include "data/ov001/data_ov001_02371ed8.inc.c"
};

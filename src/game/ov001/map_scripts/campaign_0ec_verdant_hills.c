#include "types.h"
#include "game/event_script.h"
#include "game/map_script.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Ov001_HasNoPlayer3Megatank(void);
void Player1Team_HasPlayerInGame(void);
extern EventTrigger data_ov001_0235dea4[];
extern EventScriptCommand data_ov001_0235deec[];
extern EventScriptCommand data_ov001_0235e14c[];
extern EventScriptCommand data_ov001_0235e1ec[];
extern EventScriptCommand data_ov001_0235e2ac[];
extern EventScriptCommand data_ov001_0235e52c[];
extern EventScriptCommand Ov001_gDefeatOtherTeams3Script[];
extern EventTrigger data_ov001_0235de84[];
extern EventScriptCommand data_ov001_0235df8c[];
extern EventScriptCommand data_ov001_0235e00c[];
extern EventScriptCommand data_ov001_0235e0ac[];
extern EventScriptCommand data_ov001_0235e39c[];
extern EventScriptCommand data_ov001_0235e6bc[];
extern EventTrigger data_ov001_0235df3c[];

EventTrigger data_ov001_0235de60[] = {
#include "data/ov001/data_ov001_0235de60.inc.c"
};
EventTrigger data_ov001_0235de58[] = {
#include "data/ov001/data_ov001_0235de58.inc.c"
};
EventTrigger data_ov001_0235de50[] = {
#include "data/ov001/data_ov001_0235de50.inc.c"
};
MapScript Ov001_gMapScript_0ec = {data_ov001_0235dea4,
                                  data_ov001_0235de50,
                                  data_ov001_0235de60,
                                  data_ov001_0235de84,
                                  data_ov001_0235de58,
                                  data_ov001_0235df3c,
                                  NULL};
EventTrigger data_ov001_0235de84[] = {
#include "data/ov001/data_ov001_0235de84.inc.c"
};
EventTrigger data_ov001_0235dea4[] = {
#include "data/ov001/data_ov001_0235dea4.inc.c"
};
EventScriptCommand data_ov001_0235deec[] = {
#include "data/ov001/data_ov001_0235deec.inc.c"
};
EventTrigger data_ov001_0235df3c[] = {
#include "data/ov001/data_ov001_0235df3c.inc.c"
};
EventScriptCommand data_ov001_0235df8c[] = {
#include "data/ov001/data_ov001_0235df8c.inc.c"
};
EventScriptCommand data_ov001_0235e14c[] = {
#include "data/ov001/data_ov001_0235e14c.inc.c"
};
EventScriptCommand data_ov001_0235e00c[] = {
#include "data/ov001/data_ov001_0235e00c.inc.c"
};
EventScriptCommand data_ov001_0235e0ac[] = {
#include "data/ov001/data_ov001_0235e0ac.inc.c"
};
EventScriptCommand data_ov001_0235e1ec[] = {
#include "data/ov001/data_ov001_0235e1ec.inc.c"
};
EventScriptCommand data_ov001_0235e2ac[] = {
#include "data/ov001/data_ov001_0235e2ac.inc.c"
};
EventScriptCommand data_ov001_0235e39c[] = {
#include "data/ov001/data_ov001_0235e39c.inc.c"
};
EventScriptCommand data_ov001_0235e52c[] = {
#include "data/ov001/data_ov001_0235e52c.inc.c"
};
EventScriptCommand data_ov001_0235e6bc[] = {
#include "data/ov001/data_ov001_0235e6bc.inc.c"
};

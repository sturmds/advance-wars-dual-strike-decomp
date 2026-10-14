#include "types.h"
#include "game/event_script.h"
#include "game/map_script.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Ov001_CurrentPlayerHasWaitedFootUnit(void);
void Player1Team_HasPlayerInGame(void);
extern EventTrigger data_ov001_023589ac[];
extern EventScriptCommand data_ov001_02358abc[];
extern EventScriptCommand data_ov001_02358b1c[];
extern EventScriptCommand data_ov001_02358b7c[];
extern EventScriptCommand data_ov001_02358bdc[];
extern EventScriptCommand data_ov001_02358cdc[];
extern EventScriptCommand data_ov001_023590bc[];
extern EventTrigger data_ov001_023589fc[];
extern EventScriptCommand data_ov001_02358dec[];
extern EventScriptCommand data_ov001_02358f3c[];
extern EventTrigger data_ov001_02358a34[];
extern EventTrigger data_ov001_023589cc[];
extern EventTrigger data_ov001_02358a6c[];

EventTrigger data_ov001_02358988[] = {
#include "data/ov001/data_ov001_02358988.inc.c"
};
MapScript Ov001_gMapScript_0e6 = {data_ov001_023589ac,
                                  data_ov001_023589fc,
                                  data_ov001_02358a34,
                                  data_ov001_02358988,
                                  data_ov001_023589cc,
                                  data_ov001_02358a6c,
                                  NULL};
EventTrigger data_ov001_023589ac[] = {
#include "data/ov001/data_ov001_023589ac.inc.c"
};
EventTrigger data_ov001_023589cc[] = {
#include "data/ov001/data_ov001_023589cc.inc.c"
};
EventTrigger data_ov001_02358a34[] = {
#include "data/ov001/data_ov001_02358a34.inc.c"
};
EventTrigger data_ov001_023589fc[] = {
#include "data/ov001/data_ov001_023589fc.inc.c"
};
EventTrigger data_ov001_02358a6c[] = {
#include "data/ov001/data_ov001_02358a6c.inc.c"
};
EventScriptCommand data_ov001_02358b7c[] = {
#include "data/ov001/data_ov001_02358b7c.inc.c"
};
EventScriptCommand data_ov001_02358abc[] = {
#include "data/ov001/data_ov001_02358abc.inc.c"
};
EventScriptCommand data_ov001_02358b1c[] = {
#include "data/ov001/data_ov001_02358b1c.inc.c"
};
EventScriptCommand data_ov001_02358bdc[] = {
#include "data/ov001/data_ov001_02358bdc.inc.c"
};
EventScriptCommand data_ov001_02358cdc[] = {
#include "data/ov001/data_ov001_02358cdc.inc.c"
};
EventScriptCommand data_ov001_02358dec[] = {
#include "data/ov001/data_ov001_02358dec.inc.c"
};
EventScriptCommand data_ov001_02358f3c[] = {
#include "data/ov001/data_ov001_02358f3c.inc.c"
};
EventScriptCommand data_ov001_023590bc[] = {
#include "data/ov001/data_ov001_023590bc.inc.c"
};

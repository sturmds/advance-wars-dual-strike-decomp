#include "types.h"
#include "game/event_script.h"
#include "game/map_script.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Ov001_IsMoveDestPlayer1Airport(void);
void Ov001_Player2HasNoPiperunner(void);
void Player1Team_HasPlayerInGame(void);
extern EventTrigger data_ov001_02359468[];
extern EventScriptCommand data_ov001_023595d8[];
extern EventScriptCommand data_ov001_02359678[];
extern EventScriptCommand data_ov001_023597b8[];
extern EventScriptCommand data_ov001_02359878[];
extern EventScriptCommand data_ov001_02359988[];
extern EventScriptCommand data_ov001_02359d78[];
extern EventTrigger data_ov001_023594d8[];
extern EventScriptCommand data_ov001_02359578[];
extern EventScriptCommand data_ov001_02359718[];
extern EventScriptCommand data_ov001_02359ac8[];
extern EventScriptCommand data_ov001_02359c08[];
extern EventTrigger data_ov001_023594a0[];
extern EventTrigger data_ov001_02359528[];

EventTrigger data_ov001_0235943c[] = {
#include "data/ov001/data_ov001_0235943c.inc.c"
};
EventTrigger data_ov001_02359444[] = {
#include "data/ov001/data_ov001_02359444.inc.c"
};
MapScript Ov001_gMapScript_0e7 = {data_ov001_02359468,
                                  data_ov001_023594d8,
                                  data_ov001_02359444,
                                  data_ov001_023594a0,
                                  data_ov001_0235943c,
                                  data_ov001_02359528,
                                  NULL};
EventTrigger data_ov001_023594a0[] = {
#include "data/ov001/data_ov001_023594a0.inc.c"
};
EventTrigger data_ov001_02359468[] = {
#include "data/ov001/data_ov001_02359468.inc.c"
};
EventTrigger data_ov001_023594d8[] = {
#include "data/ov001/data_ov001_023594d8.inc.c"
};
EventTrigger data_ov001_02359528[] = {
#include "data/ov001/data_ov001_02359528.inc.c"
};
EventScriptCommand data_ov001_02359578[] = {
#include "data/ov001/data_ov001_02359578.inc.c"
};
EventScriptCommand data_ov001_02359678[] = {
#include "data/ov001/data_ov001_02359678.inc.c"
};
EventScriptCommand data_ov001_02359718[] = {
#include "data/ov001/data_ov001_02359718.inc.c"
};
EventScriptCommand data_ov001_023595d8[] = {
#include "data/ov001/data_ov001_023595d8.inc.c"
};
EventScriptCommand data_ov001_023597b8[] = {
#include "data/ov001/data_ov001_023597b8.inc.c"
};
EventScriptCommand data_ov001_02359878[] = {
#include "data/ov001/data_ov001_02359878.inc.c"
};
EventScriptCommand data_ov001_02359ac8[] = {
#include "data/ov001/data_ov001_02359ac8.inc.c"
};
EventScriptCommand data_ov001_02359988[] = {
#include "data/ov001/data_ov001_02359988.inc.c"
};
EventScriptCommand data_ov001_02359c08[] = {
#include "data/ov001/data_ov001_02359c08.inc.c"
};
EventScriptCommand data_ov001_02359d78[] = {
#include "data/ov001/data_ov001_02359d78.inc.c"
};

#include "types.h"
#include "game/event_script.h"
#include "game/map_script.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Ov001_IsTileOwnedByPlayer1_x7y6(void);
void Player1Team_HasPlayerInGame(void);
extern EventTrigger data_ov001_0235c52c[];
extern EventScriptCommand data_ov001_0235c634[];
extern EventScriptCommand data_ov001_0235c774[];
extern EventScriptCommand data_ov001_0235c814[];
extern EventScriptCommand data_ov001_0235c8c4[];
extern EventTrigger data_ov001_0235c4bc[];
extern EventScriptCommand data_ov001_0235c5b4[];
extern EventScriptCommand data_ov001_0235c6d4[];
extern EventScriptCommand data_ov001_0235ca74[];
extern EventScriptCommand data_ov001_0235cc34[];
extern EventTrigger data_ov001_0235c4dc[];
extern EventTrigger data_ov001_0235c564[];

EventTrigger data_ov001_0235c490[] = {
#include "data/ov001/data_ov001_0235c490.inc.c"
};
EventTrigger data_ov001_0235c498[] = {
#include "data/ov001/data_ov001_0235c498.inc.c"
};
/* Map 0x0fd, a second header with the same name, points here too. */
MapScript Ov001_gMapScript_0ea = {data_ov001_0235c52c,
                                  data_ov001_0235c498,
                                  data_ov001_0235c4bc,
                                  data_ov001_0235c4dc,
                                  data_ov001_0235c490,
                                  data_ov001_0235c564,
                                  NULL};
EventTrigger data_ov001_0235c4dc[] = {
#include "data/ov001/data_ov001_0235c4dc.inc.c"
};
EventTrigger data_ov001_0235c4bc[] = {
#include "data/ov001/data_ov001_0235c4bc.inc.c"
};
EventScriptCommand data_ov001_0235c4fc[] = {
#include "data/ov001/data_ov001_0235c4fc.inc.c"
};
EventTrigger data_ov001_0235c52c[] = {
#include "data/ov001/data_ov001_0235c52c.inc.c"
};
EventTrigger data_ov001_0235c564[] = {
#include "data/ov001/data_ov001_0235c564.inc.c"
};
EventScriptCommand data_ov001_0235c5b4[] = {
#include "data/ov001/data_ov001_0235c5b4.inc.c"
};
EventScriptCommand data_ov001_0235c634[] = {
#include "data/ov001/data_ov001_0235c634.inc.c"
};
EventScriptCommand data_ov001_0235c6d4[] = {
#include "data/ov001/data_ov001_0235c6d4.inc.c"
};
EventScriptCommand data_ov001_0235c774[] = {
#include "data/ov001/data_ov001_0235c774.inc.c"
};
EventScriptCommand data_ov001_0235c814[] = {
#include "data/ov001/data_ov001_0235c814.inc.c"
};
EventScriptCommand data_ov001_0235c8c4[] = {
#include "data/ov001/data_ov001_0235c8c4.inc.c"
};
EventScriptCommand data_ov001_0235ca74[] = {
#include "data/ov001/data_ov001_0235ca74.inc.c"
};
EventScriptCommand data_ov001_0235cc34[] = {
#include "data/ov001/data_ov001_0235cc34.inc.c"
};

#include "types.h"
#include "game/event_script.h"
#include "game/map_script.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Ov001_MapLacksObjectKind10(void);
void Player1Team_HasPlayerInGame(void);
extern EventScriptCommand Ov001_gDefeatOtherTeams4Script[];
extern EventTrigger data_ov001_02364d1c[];
extern EventScriptCommand data_ov001_02364d8c[];
extern EventScriptCommand data_ov001_02364e0c[];
extern EventScriptCommand data_ov001_02364f4c[];
extern EventScriptCommand data_ov001_02364fec[];
extern EventScriptCommand data_ov001_0236530c[];
extern EventScriptCommand data_ov001_0236551c[];
extern EventScriptCommand data_ov001_023657dc[];
extern EventScriptCommand data_ov001_02365afc[];
extern EventScriptCommand data_ov001_0236604c[];
void func_020f2134(void);
void func_020f216c(void);
void func_ov001_023516c8(void);
void func_ov001_0235172c(void);
void func_ov001_02351738(void);
extern EventScriptCommand Ov001_gDefeatTeam4Script[];
extern EventTrigger data_ov001_02364bbc[];
extern EventScriptCommand data_ov001_02364eac[];
extern EventScriptCommand data_ov001_0236508c[];
extern EventScriptCommand data_ov001_0236512c[];
extern EventScriptCommand data_ov001_0236526c[];
extern EventScriptCommand data_ov001_0236546c[];
extern EventScriptCommand data_ov001_0236568c[];
extern EventScriptCommand data_ov001_0236594c[];
extern EventTrigger data_ov001_02364cc4[];
void func_ov001_02351708(void);
extern EventTrigger data_ov001_02364c74[];
extern EventScriptCommand data_ov001_023653bc[];
extern EventTrigger data_ov001_023655cc[];

EventTrigger data_ov001_02364ad0[] = {
#include "data/ov001/data_ov001_02364ad0.inc.c"
};
EventTrigger data_ov001_02364ad8[] = {
#include "data/ov001/data_ov001_02364ad8.inc.c"
};
MapScript data_ov001_02364af0 = {data_ov001_02364d1c, data_ov001_02364bbc,
                                 data_ov001_02364ad0, data_ov001_02364cc4,
                                 data_ov001_02364ad8, data_ov001_02364c74,
                                 data_ov001_023655cc};
EventScriptCommand data_ov001_02364b0c[] = {
#include "data/ov001/data_ov001_02364b0c.inc.c"
};
EventScriptCommand data_ov001_02364b6c[] = {
#include "data/ov001/data_ov001_02364b6c.inc.c"
};
EventScriptCommand data_ov001_02364b4c[] = {
#include "data/ov001/data_ov001_02364b4c.inc.c"
};
EventScriptCommand data_ov001_02364b2c[] = {
#include "data/ov001/data_ov001_02364b2c.inc.c"
};
EventScriptCommand data_ov001_02364b8c[] = {
#include "data/ov001/data_ov001_02364b8c.inc.c"
};
EventTrigger data_ov001_02364bbc[] = {
#include "data/ov001/data_ov001_02364bbc.inc.c"
};
EventScriptCommand data_ov001_02364bf4[] = {
#include "data/ov001/data_ov001_02364bf4.inc.c"
};
EventScriptCommand data_ov001_02364c34[] = {
#include "data/ov001/data_ov001_02364c34.inc.c"
};
EventTrigger data_ov001_02364c74[] = {
#include "data/ov001/data_ov001_02364c74.inc.c"
};
EventTrigger data_ov001_02364cc4[] = {
#include "data/ov001/data_ov001_02364cc4.inc.c"
};
EventTrigger data_ov001_02364d1c[] = {
#include "data/ov001/data_ov001_02364d1c.inc.c"
};
EventScriptCommand data_ov001_02364d8c[] = {
#include "data/ov001/data_ov001_02364d8c.inc.c"
};
EventScriptCommand data_ov001_023651cc[] = {
#include "data/ov001/data_ov001_023651cc.inc.c"
};
EventScriptCommand data_ov001_0236526c[] = {
#include "data/ov001/data_ov001_0236526c.inc.c"
};
EventScriptCommand data_ov001_02364e0c[] = {
#include "data/ov001/data_ov001_02364e0c.inc.c"
};
EventScriptCommand data_ov001_02364eac[] = {
#include "data/ov001/data_ov001_02364eac.inc.c"
};
EventScriptCommand data_ov001_02364f4c[] = {
#include "data/ov001/data_ov001_02364f4c.inc.c"
};
EventScriptCommand data_ov001_02364fec[] = {
#include "data/ov001/data_ov001_02364fec.inc.c"
};
EventScriptCommand data_ov001_0236508c[] = {
#include "data/ov001/data_ov001_0236508c.inc.c"
};
EventScriptCommand data_ov001_0236512c[] = {
#include "data/ov001/data_ov001_0236512c.inc.c"
};
EventScriptCommand data_ov001_0236546c[] = {
#include "data/ov001/data_ov001_0236546c.inc.c"
};
EventScriptCommand data_ov001_0236551c[] = {
#include "data/ov001/data_ov001_0236551c.inc.c"
};
EventScriptCommand data_ov001_0236530c[] = {
#include "data/ov001/data_ov001_0236530c.inc.c"
};
EventScriptCommand data_ov001_023653bc[] = {
#include "data/ov001/data_ov001_023653bc.inc.c"
};
EventTrigger data_ov001_023655cc[] = {
#include "data/ov001/data_ov001_023655cc.inc.c"
};
EventScriptCommand data_ov001_0236568c[] = {
#include "data/ov001/data_ov001_0236568c.inc.c"
};
EventScriptCommand data_ov001_023657dc[] = {
#include "data/ov001/data_ov001_023657dc.inc.c"
};
EventScriptCommand data_ov001_0236594c[] = {
#include "data/ov001/data_ov001_0236594c.inc.c"
};
EventScriptCommand data_ov001_02365afc[] = {
#include "data/ov001/data_ov001_02365afc.inc.c"
};

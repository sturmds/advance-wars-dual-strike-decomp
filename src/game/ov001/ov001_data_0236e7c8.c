#include "types.h"
#include "game/event_script.h"
#include "game/map_script.h"

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Ov001_MapLacksObjectKinds11To13(void);
void Ov001_SpawnOoziumAt15_10(void);
void Ov001_SpawnOoziumAt3_10(void);
void Ov001_SpawnOoziumAt9_12(void);
void Ov001_StartUnitTaskAt15_10(void);
void Ov001_StartUnitTaskAt3_10(void);
void Ov001_StartUnitTaskAt9_12(void);
void Player1Team_HasPlayerInGame(void);
void ScreenShake_Stop(void);
extern u8 data_02157480[];
extern EventScriptCommand Ov001_gDefeatOtherTeams2Script[];
extern EventScriptCommand data_ov001_0236e90c[];
extern EventScriptCommand data_ov001_0236e95c[];
extern EventScriptCommand data_ov001_0236e9ac[];
extern EventTrigger data_ov001_0236e9fc[];
extern EventScriptCommand data_ov001_0236ef9c[];
extern EventScriptCommand data_ov001_0236f17c[];
extern EventScriptCommand data_ov001_0236f23c[];
extern EventScriptCommand data_ov001_0236f38c[];
extern EventScriptCommand data_ov001_0236fc6c[];
extern EventScriptCommand data_ov001_023704fc[];
void func_0201993c(void);
void func_ov001_02351c58(void);
void func_ov001_02351cc8(void);
void func_ov001_02351d3c(void);
void func_ov001_02351d50(void);
void func_ov001_02351d74(void);
void func_ov001_02351d9c(void);
void func_ov001_02351dc8(void);
void func_ov001_02351de8(void);
void Ov001_CanSpawnOoziumAt15_10(void);
void Ov001_CanSpawnOoziumAt3_10(void);
void Ov001_CanSpawnOoziumAt9_12(void);
extern EventScriptCommand Ov001_gDefeatTeam2Script[];
extern EventTrigger data_ov001_0236ea54[];
extern EventScriptCommand data_ov001_0236eabc[];
extern EventScriptCommand data_ov001_0236f4ec[];
extern EventScriptCommand data_ov001_02370dec[];
void Ov001_MapLacksObjectKind9(void);
extern EventTrigger data_ov001_0236e884[];
extern EventScriptCommand data_ov001_023522a0[];
extern EventTrigger data_ov001_0236e8bc[];

EventTrigger data_ov001_0236e7c8[] = {
#include "data/ov001/data_ov001_0236e7c8.inc.c"
};
EventTrigger data_ov001_0236e7d0[] = {
#include "data/ov001/data_ov001_0236e7d0.inc.c"
};
MapScript data_ov001_0236e7d8 = {data_ov001_0236e9fc,
                                 data_ov001_0236ea54,
                                 data_ov001_0236e7c8,
                                 data_ov001_0236e884,
                                 data_ov001_0236e7d0,
                                 data_ov001_0236e8bc,
                                 NULL};
EventScriptCommand data_ov001_0236e7f4[] = {
#include "data/ov001/data_ov001_0236e7f4.inc.c"
};
EventScriptCommand data_ov001_0236e834[] = {
#include "data/ov001/data_ov001_0236e834.inc.c"
};
EventScriptCommand data_ov001_0236e814[] = {
#include "data/ov001/data_ov001_0236e814.inc.c"
};
EventScriptCommand data_ov001_0236e854[] = {
#include "data/ov001/data_ov001_0236e854.inc.c"
};
EventTrigger data_ov001_0236e884[] = {
#include "data/ov001/data_ov001_0236e884.inc.c"
};
EventTrigger data_ov001_0236e8bc[] = {
#include "data/ov001/data_ov001_0236e8bc.inc.c"
};
EventScriptCommand data_ov001_0236e90c[] = {
#include "data/ov001/data_ov001_0236e90c.inc.c"
};
EventScriptCommand data_ov001_0236e95c[] = {
#include "data/ov001/data_ov001_0236e95c.inc.c"
};
EventScriptCommand data_ov001_0236e9ac[] = {
#include "data/ov001/data_ov001_0236e9ac.inc.c"
};
EventTrigger data_ov001_0236e9fc[] = {
#include "data/ov001/data_ov001_0236e9fc.inc.c"
};
EventTrigger data_ov001_0236ea54[] = {
#include "data/ov001/data_ov001_0236ea54.inc.c"
};
EventScriptCommand data_ov001_0236eabc[] = {
#include "data/ov001/data_ov001_0236eabc.inc.c"
};
EventScriptCommand data_ov001_0236ef9c[] = {
#include "data/ov001/data_ov001_0236ef9c.inc.c"
};
EventScriptCommand data_ov001_0236f03c[] = {
#include "data/ov001/data_ov001_0236f03c.inc.c"
};
EventScriptCommand data_ov001_0236eb3c[] = {
#include "data/ov001/data_ov001_0236eb3c.inc.c"
};
EventScriptCommand data_ov001_0236ebdc[] = {
#include "data/ov001/data_ov001_0236ebdc.inc.c"
};
EventScriptCommand data_ov001_0236ec7c[] = {
#include "data/ov001/data_ov001_0236ec7c.inc.c"
};
EventScriptCommand data_ov001_0236ed1c[] = {
#include "data/ov001/data_ov001_0236ed1c.inc.c"
};
EventScriptCommand data_ov001_0236edbc[] = {
#include "data/ov001/data_ov001_0236edbc.inc.c"
};
EventScriptCommand data_ov001_0236ee5c[] = {
#include "data/ov001/data_ov001_0236ee5c.inc.c"
};
EventScriptCommand data_ov001_0236eefc[] = {
#include "data/ov001/data_ov001_0236eefc.inc.c"
};
EventScriptCommand data_ov001_0236f0dc[] = {
#include "data/ov001/data_ov001_0236f0dc.inc.c"
};
EventScriptCommand data_ov001_0236f17c[] = {
#include "data/ov001/data_ov001_0236f17c.inc.c"
};
EventScriptCommand data_ov001_0236f23c[] = {
#include "data/ov001/data_ov001_0236f23c.inc.c"
};
EventScriptCommand data_ov001_0236f38c[] = {
#include "data/ov001/data_ov001_0236f38c.inc.c"
};
EventScriptCommand data_ov001_0236f4ec[] = {
#include "data/ov001/data_ov001_0236f4ec.inc.c"
};
EventScriptCommand data_ov001_0236f67c[] = {
#include "data/ov001/data_ov001_0236f67c.inc.c"
};
EventScriptCommand data_ov001_0236f88c[] = {
#include "data/ov001/data_ov001_0236f88c.inc.c"
};
EventScriptCommand data_ov001_0236fc6c[] = {
#include "data/ov001/data_ov001_0236fc6c.inc.c"
};

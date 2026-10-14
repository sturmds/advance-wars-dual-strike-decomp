#include "types.h"

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
extern u8 data_ov001_02352380[];
extern u32 data_ov001_0236e90c[];
extern u32 data_ov001_0236e95c[];
extern u32 data_ov001_0236e9ac[];
extern u32 data_ov001_0236e9fc[];
extern u32 data_ov001_0236ef9c[];
extern u32 data_ov001_0236f17c[];
extern u32 data_ov001_0236f23c[];
extern u32 data_ov001_0236f38c[];
extern u32 data_ov001_0236fc6c[];
extern u8 data_ov001_023704fc[];
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
extern u8 data_ov001_02352360[];
extern u32 data_ov001_0236ea54[];
extern u32 data_ov001_0236eabc[];
extern u32 data_ov001_0236f4ec[];
extern u8 data_ov001_02370dec[];
void Ov001_MapLacksObjectKind9(void);
extern u32 data_ov001_0236e884[];
extern u8 data_ov001_023522a0[];
extern u32 data_ov001_0236e8bc[];

u32 data_ov001_0236e7c8[2] = {
#include "data/ov001/data_ov001_0236e7c8.inc.c"
};
u32 data_ov001_0236e7d0[2] = {
#include "data/ov001/data_ov001_0236e7d0.inc.c"
};
void* data_ov001_0236e7d8[7] = {&data_ov001_0236e9fc,
                                &data_ov001_0236ea54,
                                &data_ov001_0236e7c8,
                                &data_ov001_0236e884,
                                &data_ov001_0236e7d0,
                                &data_ov001_0236e8bc,
                                0};
u32 data_ov001_0236e7f4[8] = {
#include "data/ov001/data_ov001_0236e7f4.inc.c"
};
u32 data_ov001_0236e834[8] = {
#include "data/ov001/data_ov001_0236e834.inc.c"
};
u32 data_ov001_0236e814[8] = {
#include "data/ov001/data_ov001_0236e814.inc.c"
};
u32 data_ov001_0236e854[12] = {
#include "data/ov001/data_ov001_0236e854.inc.c"
};
u32 data_ov001_0236e884[14] = {
#include "data/ov001/data_ov001_0236e884.inc.c"
};
u32 data_ov001_0236e8bc[20] = {
#include "data/ov001/data_ov001_0236e8bc.inc.c"
};
u32 data_ov001_0236e90c[20] = {
#include "data/ov001/data_ov001_0236e90c.inc.c"
};
u32 data_ov001_0236e95c[20] = {
#include "data/ov001/data_ov001_0236e95c.inc.c"
};
u32 data_ov001_0236e9ac[20] = {
#include "data/ov001/data_ov001_0236e9ac.inc.c"
};
u32 data_ov001_0236e9fc[22] = {
#include "data/ov001/data_ov001_0236e9fc.inc.c"
};
u32 data_ov001_0236ea54[26] = {
#include "data/ov001/data_ov001_0236ea54.inc.c"
};
u32 data_ov001_0236eabc[32] = {
#include "data/ov001/data_ov001_0236eabc.inc.c"
};
u32 data_ov001_0236ef9c[40] = {
#include "data/ov001/data_ov001_0236ef9c.inc.c"
};
u32 data_ov001_0236f03c[40] = {
#include "data/ov001/data_ov001_0236f03c.inc.c"
};
u32 data_ov001_0236eb3c[40] = {
#include "data/ov001/data_ov001_0236eb3c.inc.c"
};
u32 data_ov001_0236ebdc[40] = {
#include "data/ov001/data_ov001_0236ebdc.inc.c"
};
u32 data_ov001_0236ec7c[40] = {
#include "data/ov001/data_ov001_0236ec7c.inc.c"
};
u32 data_ov001_0236ed1c[40] = {
#include "data/ov001/data_ov001_0236ed1c.inc.c"
};
u32 data_ov001_0236edbc[40] = {
#include "data/ov001/data_ov001_0236edbc.inc.c"
};
u32 data_ov001_0236ee5c[40] = {
#include "data/ov001/data_ov001_0236ee5c.inc.c"
};
u32 data_ov001_0236eefc[40] = {
#include "data/ov001/data_ov001_0236eefc.inc.c"
};
u32 data_ov001_0236f0dc[40] = {
#include "data/ov001/data_ov001_0236f0dc.inc.c"
};
u32 data_ov001_0236f17c[48] = {
#include "data/ov001/data_ov001_0236f17c.inc.c"
};
u32 data_ov001_0236f23c[84] = {
#include "data/ov001/data_ov001_0236f23c.inc.c"
};
u32 data_ov001_0236f38c[88] = {
#include "data/ov001/data_ov001_0236f38c.inc.c"
};
u32 data_ov001_0236f4ec[100] = {
#include "data/ov001/data_ov001_0236f4ec.inc.c"
};
u32 data_ov001_0236f67c[132] = {
#include "data/ov001/data_ov001_0236f67c.inc.c"
};
u32 data_ov001_0236f88c[248] = {
#include "data/ov001/data_ov001_0236f88c.inc.c"
};
u32 data_ov001_0236fc6c[277] = {
#include "data/ov001/data_ov001_0236fc6c.inc.c"
};

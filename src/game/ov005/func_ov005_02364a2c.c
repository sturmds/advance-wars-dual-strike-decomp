#include "types.h"
#include "game/task.h"

typedef struct Unk_02364a2c Unk_02364a2c;

struct Unk_02364a2c {
  u8 pad_00[0x4c];
  Task* unk_4c;
};

void Ov005SoundRoom_DrawBarMeter(void);
void Ov005SoundRoom_LoadLayer1Tiles(void);
void Ov005SoundRoom_LoadPictureStep(void);
void Ov005_UpdateCoPictureSwap(void);
void Ov005_UpdateMusic(void);
void Ov005_UpdatePageTurn(void);
void func_ov005_02364380(void);
void func_ov005_023646f8(void);

/* "03d": sub screen layer 1 map */
char data_ov005_0236ae74[4] = {
#include "data/ov005/data_ov005_0236ae74.inc.c"
};
/* "043": main screen OBJ palette */
char data_ov005_0236ae7c[4] = {
#include "data/ov005/data_ov005_0236ae7c.inc.c"
};
/* "03f": palette screen layer 1 map */
char data_ov005_0236ae6c[4] = {
#include "data/ov005/data_ov005_0236ae6c.inc.c"
};
/* "041": main screen OBJ tiles */
char data_ov005_0236ae78[4] = {
#include "data/ov005/data_ov005_0236ae78.inc.c"
};
/* "040": palette screen layer 1 palette */
char data_ov005_0236ae70[4] = {
#include "data/ov005/data_ov005_0236ae70.inc.c"
};
/* "03c": main screen layer 1 palette */
char data_ov005_0236ae64[4] = {
#include "data/ov005/data_ov005_0236ae64.inc.c"
};
/* "03e": palette screen layer 1 tiles */
char data_ov005_0236ae68[4] = {
#include "data/ov005/data_ov005_0236ae68.inc.c"
};
/* "03a": main screen layer 1 tiles */
char data_ov005_0236ae5c[4] = {
#include "data/ov005/data_ov005_0236ae5c.inc.c"
};
/* "03b": main screen layer 1 map */
char data_ov005_0236ae60[4] = {
#include "data/ov005/data_ov005_0236ae60.inc.c"
};
/* "042": sprite file */
char data_ov005_0236ae80[4] = {
#include "data/ov005/data_ov005_0236ae80.inc.c"
};
u32 data_ov005_0236ae8c[2] = {
#include "data/ov005/data_ov005_0236ae8c.inc.c"
};
u32 data_ov005_0236ae94[2] = {
#include "data/ov005/data_ov005_0236ae94.inc.c"
};
u32 data_ov005_0236ae9c[2] = {
#include "data/ov005/data_ov005_0236ae9c.inc.c"
};
u32 data_ov005_0236ae84[2] = {
#include "data/ov005/data_ov005_0236ae84.inc.c"
};
u32 data_ov005_0236aea4[3] = {
#include "data/ov005/data_ov005_0236aea4.inc.c"
};
u32 data_ov005_0236aeb0[5] = {
#include "data/ov005/data_ov005_0236aeb0.inc.c"
};
TaskScriptCmd data_ov005_0236aedc[3] = {
#include "data/ov005/data_ov005_0236aedc.inc.c"
};
static TaskScriptCmd data_ov005_0236aec4[3] = {
#include "data/ov005/data_ov005_0236aec4.inc.c"
};
TaskScriptCmd data_ov005_0236aef4[3] = {
#include "data/ov005/data_ov005_0236aef4.inc.c"
};
TaskScriptCmd data_ov005_0236af0c[3] = {
#include "data/ov005/data_ov005_0236af0c.inc.c"
};

Task* func_ov005_02364a2c(Unk_02364a2c* a0) {
  Task* t0 = Task_Create(data_ov005_0236aec4, TASK_LIST(3));
  a0->unk_4c = t0;
  return t0;
}

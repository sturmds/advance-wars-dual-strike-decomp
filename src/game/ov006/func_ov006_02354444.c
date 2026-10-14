#include "types.h"
#include "game/task.h"
#include "game/Unk_ov006_0235f690.h"

void func_0201e954(void);
void Gfx_ClearLayerMap(int arg);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
int Ov006_HasNonSpace(u8* text);
u8* func_021016e4(void);
void NameEntry_ClearBuffer(void);
char* strcpy(char* dest, const char* src);

extern Unk_ov006_0235f690* data_ov006_0236d640;
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Ov006_OpenPopupMenu(void);
void func_ov006_02354250(void);
void func_ov006_0235427c(void);
void func_ov006_023542a8(void);
void func_ov006_023542d4(void);
void func_ov006_02354300(void);
void func_ov006_023544c4(void);
void func_ov006_023545ac(void);
void func_ov006_023545dc(void);

static TaskScriptCmd data_ov006_0236acc4[11] = {
#include "data/ov006/data_ov006_0236acc4.inc.c"
};
u32 data_ov006_0236ae1c[32] = {
#include "data/ov006/data_ov006_0236ae1c.inc.c"
};
u32 data_ov006_0236ad9c[32] = {
#include "data/ov006/data_ov006_0236ad9c.inc.c"
};
u32 data_ov006_0236ad1c[32] = {
#include "data/ov006/data_ov006_0236ad1c.inc.c"
};
u32 data_ov006_0236ae9c[32] = {
#include "data/ov006/data_ov006_0236ae9c.inc.c"
};
u32 data_ov006_0236af1c[32] = {
#include "data/ov006/data_ov006_0236af1c.inc.c"
};

void func_ov006_02354444(void) {
  func_0201e954();
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(1);
  func_020c4018();
  if (Ov006_HasNonSpace(data_ov006_0236d640->unk_d8)) {
    strcpy((char*)func_021016e4(), (char*)data_ov006_0236d640->unk_d8);
  } else {
    NameEntry_ClearBuffer();
  }
  data_ov006_0236d640->unk_64 = Task_Create(data_ov006_0236acc4, TASK_LIST(3));
}

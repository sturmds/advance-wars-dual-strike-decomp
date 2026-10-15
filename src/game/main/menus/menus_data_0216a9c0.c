#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/event_script.h"
#include "game/menu_button.h"
#include "game/name_entry_work.h"
#include "game/screen_contexts.h"
#include "game/task.h"

void NameEntry_AllocWork(void);
void NameEntry_Destroy(void);
void NameEntry_EndScript(void);
void NameEntry_MoveFromControlKey(void);
void NameEntry_MoveFromKey(void);
void NameEntry_SetUpScreen(void);
void NameEntry_Update(void);
void NameEntry_OnControlKeyConfirm(void);
void func_02101500(void);
void NameEntry_OnControlKeySelect(void);
void NameEntry_OnKeySelect(void);

u32 data_0216a9d8 = {
#include "data/main/data_0216a9d8.inc.c"
};
u32 data_0216a9e8 = {
#include "data/main/data_0216a9e8.inc.c"
};
u32 data_0216a9dc = {
#include "data/main/data_0216a9dc.inc.c"
};
u32 data_0216a9c8 = {
#include "data/main/data_0216a9c8.inc.c"
};
u32 data_0216a9e0 = {
#include "data/main/data_0216a9e0.inc.c"
};
u32 data_0216a9e4 = {
#include "data/main/data_0216a9e4.inc.c"
};
u32 data_0216a9cc = {
#include "data/main/data_0216a9cc.inc.c"
};
u32 data_0216a9d4 = {
#include "data/main/data_0216a9d4.inc.c"
};
u32 data_0216a9ec = {
#include "data/main/data_0216a9ec.inc.c"
};
u32 data_0216a9c0 = {
#include "data/main/data_0216a9c0.inc.c"
};
u32 data_0216a9d0 = {
#include "data/main/data_0216a9d0.inc.c"
};
u32 data_0216a9c4 = {
#include "data/main/data_0216a9c4.inc.c"
};
u32 data_0216aadc = {
#include "data/main/data_0216aadc.inc.c"
};
u32 data_0216aad8 = {
#include "data/main/data_0216aad8.inc.c"
};
u32 data_0216aad4 = {
#include "data/main/data_0216aad4.inc.c"
};
u32 data_0216aad0 = {
#include "data/main/data_0216aad0.inc.c"
};
u32 data_0216aacc = {
#include "data/main/data_0216aacc.inc.c"
};
u32 data_0216aac8 = {
#include "data/main/data_0216aac8.inc.c"
};
u32 data_0216aac4 = {
#include "data/main/data_0216aac4.inc.c"
};
u32 data_0216aac0 = {
#include "data/main/data_0216aac0.inc.c"
};
u32 data_0216aabc = {
#include "data/main/data_0216aabc.inc.c"
};
u32 data_0216aab8 = {
#include "data/main/data_0216aab8.inc.c"
};
u32 data_0216aab4 = {
#include "data/main/data_0216aab4.inc.c"
};
u32 data_0216aab0 = {
#include "data/main/data_0216aab0.inc.c"
};
u32 data_0216aaac = {
#include "data/main/data_0216aaac.inc.c"
};
u32 data_0216aaa8 = {
#include "data/main/data_0216aaa8.inc.c"
};
u32 data_0216aaa4 = {
#include "data/main/data_0216aaa4.inc.c"
};
u32 data_0216aaa0 = {
#include "data/main/data_0216aaa0.inc.c"
};
u32 data_0216aa9c = {
#include "data/main/data_0216aa9c.inc.c"
};
u32 data_0216aa98 = {
#include "data/main/data_0216aa98.inc.c"
};
u32 data_0216aa94 = {
#include "data/main/data_0216aa94.inc.c"
};
u32 data_0216aa90 = {
#include "data/main/data_0216aa90.inc.c"
};
u32 data_0216aa8c = {
#include "data/main/data_0216aa8c.inc.c"
};
u32 data_0216aa88 = {
#include "data/main/data_0216aa88.inc.c"
};
u32 data_0216aa84 = {
#include "data/main/data_0216aa84.inc.c"
};
u32 data_0216aa80 = {
#include "data/main/data_0216aa80.inc.c"
};
u32 data_0216aa7c = {
#include "data/main/data_0216aa7c.inc.c"
};
u32 data_0216aa78 = {
#include "data/main/data_0216aa78.inc.c"
};
u32 data_0216aa74 = {
#include "data/main/data_0216aa74.inc.c"
};
u32 data_0216aa70 = {
#include "data/main/data_0216aa70.inc.c"
};
u32 data_0216aa6c = {
#include "data/main/data_0216aa6c.inc.c"
};
u32 data_0216aa68 = {
#include "data/main/data_0216aa68.inc.c"
};
u32 data_0216aa64 = {
#include "data/main/data_0216aa64.inc.c"
};
u32 data_0216aa60 = {
#include "data/main/data_0216aa60.inc.c"
};
u32 data_0216aa5c = {
#include "data/main/data_0216aa5c.inc.c"
};
u32 data_0216aa58 = {
#include "data/main/data_0216aa58.inc.c"
};
u32 data_0216aa54 = {
#include "data/main/data_0216aa54.inc.c"
};
u32 data_0216aa50 = {
#include "data/main/data_0216aa50.inc.c"
};
u32 data_0216aa4c = {
#include "data/main/data_0216aa4c.inc.c"
};
u32 data_0216aa48 = {
#include "data/main/data_0216aa48.inc.c"
};
u32 data_0216aa44 = {
#include "data/main/data_0216aa44.inc.c"
};
u32 data_0216aa40 = {
#include "data/main/data_0216aa40.inc.c"
};
u32 data_0216aa3c = {
#include "data/main/data_0216aa3c.inc.c"
};
u32 data_0216aa38 = {
#include "data/main/data_0216aa38.inc.c"
};
u32 data_0216aa34 = {
#include "data/main/data_0216aa34.inc.c"
};
u32 data_0216aa30 = {
#include "data/main/data_0216aa30.inc.c"
};
u32 data_0216aa2c = {
#include "data/main/data_0216aa2c.inc.c"
};
u32 data_0216aa28 = {
#include "data/main/data_0216aa28.inc.c"
};
u32 data_0216aa24 = {
#include "data/main/data_0216aa24.inc.c"
};
u32 data_0216aa20 = {
#include "data/main/data_0216aa20.inc.c"
};
u32 data_0216aa1c = {
#include "data/main/data_0216aa1c.inc.c"
};
u32 data_0216aa18 = {
#include "data/main/data_0216aa18.inc.c"
};
u32 data_0216aa14 = {
#include "data/main/data_0216aa14.inc.c"
};
u32 data_0216aa10 = {
#include "data/main/data_0216aa10.inc.c"
};
u32 data_0216aa0c = {
#include "data/main/data_0216aa0c.inc.c"
};
u32 data_0216aa08 = {
#include "data/main/data_0216aa08.inc.c"
};
u32 data_0216aa04 = {
#include "data/main/data_0216aa04.inc.c"
};
u32 data_0216aa00 = {
#include "data/main/data_0216aa00.inc.c"
};
u32 data_0216a9fc = {
#include "data/main/data_0216a9fc.inc.c"
};
u32 data_0216a9f8 = {
#include "data/main/data_0216a9f8.inc.c"
};
u32 data_0216a9f4 = {
#include "data/main/data_0216a9f4.inc.c"
};
u32 data_0216a9f0 = {
#include "data/main/data_0216a9f0.inc.c"
};
u8 data_0216aae0[] = {
#include "data/main/data_0216aae0.inc.c"
};
MenuButtonTemplate data_0216ab04 = {
#include "data/main/data_0216ab04.inc.c"
};
MenuButtonTemplate data_0216ab2c = {
#include "data/main/data_0216ab2c.inc.c"
};
EventScriptCommand data_0216abb4[] = {
#include "data/main/data_0216abb4.inc.c"
};
TaskScriptCmd gNameEntryScript[] = {
#include "data/main/gNameEntryScript.inc.c"
};
EventScriptCommand data_0216ac44[] = {
#include "data/main/data_0216ac44.inc.c"
};
EventScriptCommand data_0216ab54[] = {
#include "data/main/data_0216ab54.inc.c"
};
EventScriptCommand data_0216ab84[] = {
#include "data/main/data_0216ab84.inc.c"
};
EventScriptCommand data_0216abe4[] = {
#include "data/main/data_0216abe4.inc.c"
};
GfxLayer data_0216ac74[] = {
#include "data/main/data_0216ac74.inc.c"
};
/* tile x and y of each key label, two bytes per key */
u8 data_0216acc4[] = {
#include "data/main/data_0216acc4.inc.c"
};
u8* data_0216ad5c[] = {(u8*)&data_0216aa10,
                       (u8*)&data_0216a9c8,
                       (u8*)&data_0216a9e4,
                       (u8*)&data_0216a9d4,
                       (u8*)&data_0216a9cc,
                       (u8*)&data_0216a9c0,
                       (u8*)&data_0216a9c4,
                       (u8*)&data_0216aadc,
                       (u8*)&data_0216aad8,
                       (u8*)&data_0216aad4,
                       (u8*)&data_0216aad0,
                       (u8*)&data_0216aacc,
                       (u8*)&data_0216aac8,
                       (u8*)&data_0216aac4,
                       (u8*)&data_0216aac0,
                       (u8*)&data_0216aabc,
                       (u8*)&data_0216aab8,
                       (u8*)&data_0216aab4,
                       (u8*)&data_0216aab0,
                       (u8*)&data_0216aaac,
                       (u8*)&data_0216aaa8,
                       (u8*)&data_0216aaa4,
                       (u8*)&data_0216aaa0,
                       (u8*)&data_0216aa9c,
                       (u8*)&data_0216aa98,
                       (u8*)&data_0216aa94,
                       (u8*)&data_0216aa90,
                       (u8*)&data_0216aa8c,
                       (u8*)&data_0216aa88,
                       (u8*)&data_0216aa84,
                       (u8*)&data_0216aa80,
                       (u8*)&data_0216aa7c,
                       (u8*)&data_0216aa78,
                       (u8*)&data_0216aa74,
                       (u8*)&data_0216aa70,
                       (u8*)&data_0216aa60,
                       (u8*)&data_0216a9d8,
                       (u8*)&data_0216aa64,
                       (u8*)&data_0216aa5c,
                       (u8*)&data_0216aa08,
                       (u8*)&data_0216aa6c,
                       (u8*)&data_0216aa54,
                       (u8*)&data_0216aa50,
                       (u8*)&data_0216aa4c,
                       (u8*)&data_0216aa48,
                       (u8*)&data_0216aa44,
                       (u8*)&data_0216aa40,
                       (u8*)&data_0216aa3c,
                       (u8*)&data_0216aa38,
                       (u8*)&data_0216aa34,
                       (u8*)&data_0216aa30,
                       (u8*)&data_0216aa2c,
                       (u8*)&data_0216aa28,
                       (u8*)&data_0216aa24,
                       (u8*)&data_0216aa20,
                       (u8*)&data_0216aa1c,
                       (u8*)&data_0216aa18,
                       (u8*)&data_0216aa14,
                       (u8*)&data_0216a9d0,
                       (u8*)&data_0216aa0c,
                       (u8*)&data_0216a9e0,
                       (u8*)&data_0216aa04,
                       (u8*)&data_0216aa00,
                       (u8*)&data_0216a9fc,
                       (u8*)&data_0216a9f8,
                       (u8*)&data_0216a9f4,
                       (u8*)&data_0216a9f0,
                       (u8*)&data_0216a9ec,
                       (u8*)&data_0216a9e8,
                       (u8*)&data_0216aa68,
                       (u8*)&data_0216aa58,
                       (u8*)&data_0216a9dc,
                       (u8*)&data_0216a9dc,
                       (u8*)&data_0216a9dc,
                       (u8*)&data_0216a9dc,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0};
NameEntryKey data_0216afb4[NAME_ENTRY_KEY_COUNT] = {
#include "data/main/data_0216afb4.inc.c"
};

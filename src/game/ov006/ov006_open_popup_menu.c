#include "types.h"
#include "game/menu_item.h"
#include "game/event_script.h"
#include "game/popup_menu.h"

PopupMenuTask* PopupMenu_Create(const MenuItem* items, int x, int y, int style,
                                u16 selected, int center);
void func_ov006_02354680(PopupMenuTask* menu);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
extern u8 data_ov006_0236ac74[];
extern u8 data_ov006_0236ad1c[];
extern u8 data_ov006_0236ad9c[];
extern u8 data_ov006_0236ae1c[];
extern u8 data_ov006_0236ae9c[];
extern u8 data_ov006_0236af1c[];
void func_ov006_02350f10(void);
void func_ov006_023543e8(void);
void func_ov006_023547b4(void);
void func_ov006_023580c0(void);
void Ov006_PopupChoiceStartScriptA(void);
void Ov006_PopupChoiceStartScriptB(void);
void func_020199a4(void);
void func_ov006_02354090(void);
void func_ov006_02354108(void);
void func_ov006_02354444(void);
void func_ov006_023545ec(void);
void func_ov006_023586e0(void);
void func_ov006_02354684(void);
void func_ov006_023546a0(void);

static MenuItem data_ov006_0236af9c[] = {
#include "data/ov006/data_ov006_0236af9c.inc.c"
};
EventScriptCommand data_ov006_0236b0bc[] = {
#include "data/ov006/data_ov006_0236b0bc.inc.c"
};
MenuItem data_ov006_0236b15c[] = {
#include "data/ov006/data_ov006_0236b15c.inc.c"
};
EventScriptCommand data_ov006_0236b01c[] = {
#include "data/ov006/data_ov006_0236b01c.inc.c"
};
EventScriptCommand data_ov006_0236b2ac[] = {
#include "data/ov006/data_ov006_0236b2ac.inc.c"
};
EventScriptCommand data_ov006_0236b35c[] = {
#include "data/ov006/data_ov006_0236b35c.inc.c"
};
EventScriptCommand data_ov006_0236b1fc[] = {
#include "data/ov006/data_ov006_0236b1fc.inc.c"
};
EventScriptCommand data_ov006_0236b40c[] = {
#include "data/ov006/data_ov006_0236b40c.inc.c"
};
EventScriptCommand data_ov006_0236b4bc[] = {
#include "data/ov006/data_ov006_0236b4bc.inc.c"
};

void Ov006_OpenPopupMenu(void) {
  func_ov006_02354680(PopupMenu_Create(data_ov006_0236af9c, 2, 3, 0, 0, 0));
}

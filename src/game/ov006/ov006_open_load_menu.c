#include "types.h"
#include "game/script_slot.h"
#include "game/event_script.h"
#include "game/popup_menu.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/menu_item.h"
#include "game/screen.h"

PopupMenuTask* PopupMenu_Create(const MenuItem* items, int x, int y, int style,
                                int selected, int center);
int DesignMap_CopyName(int id, u8* buf);
void Ov006_DrawMenuLineFixedText(int unused, int row);
void Ov006_DrawMenuLineWithIcon(int unused, int row, const char* text);

void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void PopupMenu_PopState(void);
extern EventScriptCommand data_ov006_0236be00[];
extern EventScriptCommand data_ov006_0236bf20[];
void func_01ff95fc(void);
void func_01ff9610(void);
void func_0201e954(void);
void func_020d7ea4(void);
void func_ov006_02350f10(void);
void func_ov006_02357770(void);
void func_ov006_023578e8(void);
void func_ov006_02357c80(void);
void func_ov006_02357c90(void);
void func_ov006_02357cb8(void);
void func_ov006_02357d40(void);
void func_ov006_02357f84(void);
void func_ov006_02357f94(void);
void func_ov006_02357fa4(void);
void Ov006_OpenLoadMenu(void);
void Ov006_OpenSaveMenu(void);
void func_020199a4(void);
void func_020c4018(void);
void func_020d7eb8(void);
void func_ov006_02357ef4(void);
void func_ov006_023582c0(void);
void func_ov006_023582d0(void);
void func_ov006_023582e0(void);
void func_ov006_02357a98(void);
void func_ov006_02357aa8(void);
void func_ov006_02357ab8(void);
void func_ov006_02357ca0(void);
void func_ov006_02357dc8(void);
void func_ov006_02357e14(void);
void func_ov006_02357e60(void);
void func_ov006_0235869c(void);
void func_ov006_02354680(void);
void func_ov006_023578bc(void);
void func_ov006_02357a6c(void);

/* "001" */
char data_ov006_0236b8b4[] = {
#include "data/ov006/data_ov006_0236b8b4.inc.c"
};
/* "002" */
char data_ov006_0236b8b0[] = {
#include "data/ov006/data_ov006_0236b8b0.inc.c"
};
ScriptCommand data_ov006_0236b8e0[] = {
#include "data/ov006/data_ov006_0236b8e0.inc.c"
};
ScriptCommand data_ov006_0236b8b8[] = {
#include "data/ov006/data_ov006_0236b8b8.inc.c"
};
ScriptCommand data_ov006_0236b908[] = {
#include "data/ov006/data_ov006_0236b908.inc.c"
};
ScriptCommand data_ov006_0236b940[] = {
#include "data/ov006/data_ov006_0236b940.inc.c"
};
ScriptCommand data_ov006_0236b978[] = {
#include "data/ov006/data_ov006_0236b978.inc.c"
};
MenuItem data_ov006_0236ba60[] = {
#include "data/ov006/data_ov006_0236ba60.inc.c"
};
EventScriptCommand data_ov006_0236bae0[] = {
#include "data/ov006/data_ov006_0236bae0.inc.c"
};
static MenuItem data_ov006_0236b9e0[] = {
#include "data/ov006/data_ov006_0236b9e0.inc.c"
};
EventScriptCommand data_ov006_0236bb60[] = {
#include "data/ov006/data_ov006_0236bb60.inc.c"
};
EventScriptCommand data_ov006_0236bc00[] = {
#include "data/ov006/data_ov006_0236bc00.inc.c"
};
EventScriptCommand data_ov006_0236bd50[] = {
#include "data/ov006/data_ov006_0236bd50.inc.c"
};
EventScriptCommand data_ov006_0236bca0[] = {
#include "data/ov006/data_ov006_0236bca0.inc.c"
};
EventScriptCommand data_ov006_0236be00[] = {
#include "data/ov006/data_ov006_0236be00.inc.c"
};
EventScriptCommand data_ov006_0236bf20[] = {
#include "data/ov006/data_ov006_0236bf20.inc.c"
};

/* Opens the menu of the three design map slots with the cursor on the last
 * chosen slot, drawn like Ov006_OpenSaveMenu. Choosing a row starts a script
 * (func_ov006_02357eac) only when the slot holds a map (flag 0x200 << slot),
 * so this looks like the load menu. */
void Ov006_OpenLoadMenu(void) {
  /* chosenSlot is 0xff (-1 here) when none was chosen. */
  s8 slot = data_ov006_0236d640->chosenSlot;

  PopupMenu_Create(data_ov006_0236b9e0, 2, 5, 0, slot < 0 ? 0 : slot, 0);
  if (DesignMap_CopyName(0, &data_ov006_0236d640->unk_eb) != 1) {
    Ov006_DrawMenuLineFixedText(0, 0);
  } else {
    Ov006_DrawMenuLineWithIcon(0, 0, (const char*)&data_ov006_0236d640->unk_eb);
  }
  if (DesignMap_CopyName(1, &data_ov006_0236d640->unk_eb) != 1) {
    Ov006_DrawMenuLineFixedText(0, 1);
  } else {
    Ov006_DrawMenuLineWithIcon(0, 1, (const char*)&data_ov006_0236d640->unk_eb);
  }
  if (DesignMap_CopyName(2, &data_ov006_0236d640->unk_eb) != 1) {
    Ov006_DrawMenuLineFixedText(0, 2);
  } else {
    Ov006_DrawMenuLineWithIcon(0, 2, (const char*)&data_ov006_0236d640->unk_eb);
  }
}

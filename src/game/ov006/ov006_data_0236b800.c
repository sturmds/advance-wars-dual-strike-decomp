#include "types.h"
#include "game/ov006_cursor_steps.h"
#include "game/menu_button.h"

void Ov006_EntryButtonKeys(void);
void Ov006_OnToolButton(void);
void func_ov006_02355d84(void);
void func_ov006_023564b0(void);

CursorSteps26 data_ov006_0236b800 = {
#include "data/ov006/data_ov006_0236b800.inc.c"
};
CursorSteps26 data_ov006_0236b838 = {
#include "data/ov006/data_ov006_0236b838.inc.c"
};
CursorSteps26 data_ov006_0236b81c = {
#include "data/ov006/data_ov006_0236b81c.inc.c"
};
MenuButtonTemplate data_ov006_0236b854 = {
#include "data/ov006/data_ov006_0236b854.inc.c"
};
MenuButtonTemplate gOv006ToolButtonTemplate = {
#include "data/ov006/gOv006ToolButtonTemplate.inc.c"
};

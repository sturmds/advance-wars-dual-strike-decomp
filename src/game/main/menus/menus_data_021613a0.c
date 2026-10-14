#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void CombatIntro_ResetSeenFlags(void);
void CombatModeCoPortrait_Init(void);
void CombatModeMenu_Close(void);
void CombatModeMenu_DrawSprites(void);
void CombatModeMenu_Init(void);
void CombatModeMenu_OnKeys(void);
void CombatModeMenu_OpenShop(void);
void CombatModeMenu_QueueSlotSprites(void);
void CombatModeMenu_ShopOnKeys(void);
void CombatModeMenu_ShowMainLayers(void);
void CombatSetup_ResetDefaults(void);
void func_02089244(void);
void func_02089a74(void);
void func_0208a598(void);
void func_0208ad70(void);
void func_0208ad7c(void);
void func_0208ad88(void);
void func_0208ad94(void);
void func_0208ada0(void);
void func_020b983c(void);
void CombatIntro_StartFirst(void);
void CombatIntro_StartSecond(void);
void CombatModeCoPortrait_Update(void);
void CombatModeMenu_BuyUnit(void);
void CombatModeMenu_PickSlot(void);
void func_0208a594(void);
void CombatModeMenu_SellLastUnit(void);
void func_0208a128(void);
void CombatModeMenu_OnSelect(void);
void func_020892f0(void);

/* CO ids: Andy, Max, Sami, Flak, Lash, Adder, Hawke */
u8 gCombatModeCos[] = {
#include "data/main/gCombatModeCos.inc.c"
};
/* declared `IconCounts gCombatModeIconCounts` in combat_mode_menu_draw_sprites.c: declared in src/game/main/menus/combat_mode_menu_draw_sprites.c, with a type no header defines */
u32 gCombatModeIconCounts[] = {
#include "data/main/gCombatModeIconCounts.inc.c"
};
u32 data_021613c8[] = {
#include "data/main/data_021613c8.inc.c"
};
u32 data_021613b8[] = {
#include "data/main/data_021613b8.inc.c"
};
/* declared `u8 data_021613d8[]` in draw_selected_co_power_text.c: relocation at data_021613d8+0xc is not on a 4-byte field */
u32 data_021613d8[] = {
#include "data/main/data_021613d8.inc.c"
};
MenuButtonTemplate data_02161448 = {
#include "data/main/data_02161448.inc.c"
};
/* declared `u8 data_02161498[]` in func_0208ad2c.c: relocation at data_02161498+0x18 is not on a 4-byte field */
u32 data_02161498[] = {
#include "data/main/data_02161498.inc.c"
};
/* declared `u8 data_021614c0[]` in func_0208ad2c.c: relocation at data_021614c0+0x18 is not on a 4-byte field */
u32 data_021614c0[] = {
#include "data/main/data_021614c0.inc.c"
};
MenuButtonTemplate data_021613f8 = {
#include "data/main/data_021613f8.inc.c"
};
/* declared `u8 data_02161420[]` in func_0208acb0.c: relocation at data_02161420+0x18 is not on a 4-byte field */
u32 data_02161420[] = {
#include "data/main/data_02161420.inc.c"
};
MenuButtonTemplate data_02161470 = {
#include "data/main/data_02161470.inc.c"
};
MenuButtonTemplate gCombatModeSlotButton = {
#include "data/main/gCombatModeSlotButton.inc.c"
};
/* declared `u8 data_02161510[]` in func_0208a128.c: relocation at data_02161510+0x4 is not on a 4-byte field */
u32 data_02161510[] = {
#include "data/main/data_02161510.inc.c"
};
/* declared `u8 data_02161570[]` in func_02089260.c: relocation at data_02161570+0x4 is not on a 4-byte field */
u32 data_02161570[] = {
#include "data/main/data_02161570.inc.c"
};
TaskScriptCmd data_021615d0[] = {
#include "data/main/data_021615d0.inc.c"
};

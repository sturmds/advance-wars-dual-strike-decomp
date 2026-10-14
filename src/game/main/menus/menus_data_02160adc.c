#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void CombatSetup_AnimatePalette(void);
void CombatSetup_ClearRowScreens(void);
void CombatSetup_ClearScreens(void);
void CombatSetup_DrawRowSprites(void);
void CombatSetup_EndLink(void);
void CombatSetup_Init(void);
void CombatSetup_InitCoPickScreen(void);
void CombatSetup_InitScreen(void);
void CombatSetup_PressReady(void);
void CombatSetup_PressRowButton(void);
void CombatSetup_QueueIconsAndSettings(void);
void CombatSetup_QueueRowSprites(void);
void CombatSetup_ResetMainScreen(void);
void CombatSetup_ShopOnKeys(void);
void CombatSetup_StartVariantSync(void);
void LinkMenu_DrawSettings(void);
void LinkMenu_OpenSettings(void);
void LinkMenu_RestoreMainScreen(void);
void LinkMenu_Update(void);
void RowLockMark_InitRow1(void);
void RowLockMark_InitRow2(void);
void RowLockMark_InitRow3(void);
void RowLockMark_InitRow4(void);
void RowLockMark_PopIn(void);
void RowLockMark_ResetFrame(void);
void RowLockMark_SquashOut(void);
void SubScreen_ResetMenu(void);
void func_02082ac8(void);
void func_02083020(void);
void func_0208303c(void);
void func_02083064(void);
void func_020837ec(void);
void func_02083814(void);
void func_02084c6c(void);
void func_02084c78(void);
void func_0208588c(void);
void func_02085898(void);
void func_020858a4(void);
void func_020858b0(void);
void func_02086754(void);
void func_020876e4(void);
void func_020876f0(void);
void func_020876fc(void);
void func_02087714(void);
void func_0208773c(void);
void func_02087828(void);
void func_020878c0(void);
void func_0208798c(void);
void func_02087a44(void);
void func_02087aa8(void);
void func_02087c20(void);
void func_02087cd4(void);
void func_02088404(void);
void func_02088478(void);
void func_0208854c(void);
void func_02088b3c(void);
void func_02088c14(void);
void func_02088c20(void);
void func_02088c2c(void);
void func_020b983c(void);
void CombatSetup_AddLeaderButtons(void);
void CombatSetup_AddRowButtons(void);
void CombatSetup_AddShopButtons(void);
void LinkMenu_RestoreButtons(void);
void func_02083800(void);
void func_02087808(void);
void func_02087b88(void);
void CombatSetup_UpdateCoPick(void);
void CombatSetup_UpdateLink(void);
void CombatSetup_UpdateShop(void);
void func_020877f4(void);
void func_020877e0(void);

u8 data_02160adc[] = {
#include "data/main/data_02160adc.inc.c"
};
u8 data_02160ae0[] = {
#include "data/main/data_02160ae0.inc.c"
};
char data_02160ae8[] = {
#include "data/main/data_02160ae8.inc.c"
};
/* x, y of each of the 5 entries; needed to match: not const */
u8 data_02160af0[] = {
#include "data/main/data_02160af0.inc.c"
};
u32 data_02160b2c[] = {
#include "data/main/data_02160b2c.inc.c"
};
u32 data_02160b1c[] = {
#include "data/main/data_02160b1c.inc.c"
};
/* declared `UnitCountTable data_02160afc` in combat_setup_draw_row_sprites.c: declared in src/game/main/menus/combat_setup_draw_row_sprites.c, with a type no header defines */
u32 data_02160afc[] = {
#include "data/main/data_02160afc.inc.c"
};
u32 data_02160b3c[] = {
#include "data/main/data_02160b3c.inc.c"
};
u32 data_02160b0c[] = {
#include "data/main/data_02160b0c.inc.c"
};
u32 data_02160b5c[] = {
#include "data/main/data_02160b5c.inc.c"
};
u32 data_02160b4c[] = {
#include "data/main/data_02160b4c.inc.c"
};
u32 data_02160b6c[] = {
#include "data/main/data_02160b6c.inc.c"
};
TaskScriptCmd data_02160b84[] = {
#include "data/main/data_02160b84.inc.c"
};
u8 data_02160b9c[] = {
#include "data/main/data_02160b9c.inc.c"
};
TaskScriptCmd data_02160bb8[] = {
#include "data/main/data_02160bb8.inc.c"
};
/* declared `u8 data_02160db8[]` in combat_setup_add_shop_buttons.c: relocation at data_02160db8+0x14 is not on a 4-byte field */
u32 data_02160db8[] = {
#include "data/main/data_02160db8.inc.c"
};
MenuButtonTemplate data_02160c00 = {
#include "data/main/data_02160c00.inc.c"
};
MenuButtonTemplate data_02160d68 = {
#include "data/main/data_02160d68.inc.c"
};
MenuButtonTemplate data_02160d40 = {
#include "data/main/data_02160d40.inc.c"
};
MenuButtonTemplate data_02160c78 = {
#include "data/main/data_02160c78.inc.c"
};
MenuButtonTemplate data_02160ca0 = {
#include "data/main/data_02160ca0.inc.c"
};
MenuButtonTemplate data_02160cf0 = {
#include "data/main/data_02160cf0.inc.c"
};
MenuButtonTemplate data_02160d18 = {
#include "data/main/data_02160d18.inc.c"
};
MenuButtonTemplate data_02160de0 = {
#include "data/main/data_02160de0.inc.c"
};
MenuButtonTemplate data_02160e08 = {
#include "data/main/data_02160e08.inc.c"
};
MenuButtonTemplate data_02160e30 = {
#include "data/main/data_02160e30.inc.c"
};
/* declared `u8 data_02160e58[]` in combat_setup_add_shop_buttons.c: relocation at data_02160e58+0x18 is not on a 4-byte field */
u32 data_02160e58[] = {
#include "data/main/data_02160e58.inc.c"
};
MenuButtonTemplate data_02160e80 = {
#include "data/main/data_02160e80.inc.c"
};
MenuButtonTemplate data_02160ea8 = {
#include "data/main/data_02160ea8.inc.c"
};
/* declared `u8 data_02160bd8[]` in combat_setup_add_shop_buttons.c: relocation at data_02160bd8+0x18 is not on a 4-byte field */
u32 data_02160bd8[] = {
#include "data/main/data_02160bd8.inc.c"
};
MenuButtonTemplate data_02160c28 = {
#include "data/main/data_02160c28.inc.c"
};
MenuButtonTemplate data_02160c50 = {
#include "data/main/data_02160c50.inc.c"
};
MenuButtonTemplate data_02160cc8 = {
#include "data/main/data_02160cc8.inc.c"
};
MenuButtonTemplate data_02160d90 = {
#include "data/main/data_02160d90.inc.c"
};
TaskScriptCmd gRowLockMarkRow2Script[] = {
#include "data/main/gRowLockMarkRow2Script.inc.c"
};
TaskScriptCmd gRowLockMarkRow3Script[] = {
#include "data/main/gRowLockMarkRow3Script.inc.c"
};
TaskScriptCmd gRowLockMarkRow4Script[] = {
#include "data/main/gRowLockMarkRow4Script.inc.c"
};
TaskScriptCmd gRowLockMarkRow1Script[] = {
#include "data/main/gRowLockMarkRow1Script.inc.c"
};
u32 data_02160f90[] = {
#include "data/main/data_02160f90.inc.c"
};
u32 data_02160ff0[] = {
#include "data/main/data_02160ff0.inc.c"
};
u32 data_02161050[] = {
#include "data/main/data_02161050.inc.c"
};
u32 data_021610b0[] = {
#include "data/main/data_021610b0.inc.c"
};
u32 data_02161150[] = {
#include "data/main/data_02161150.inc.c"
};
TaskScriptCmd gCombatSetupTaskScript[] = {
#include "data/main/gCombatSetupTaskScript.inc.c"
};

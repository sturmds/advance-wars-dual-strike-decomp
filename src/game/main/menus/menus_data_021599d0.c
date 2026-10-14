#include "types.h"
#include "game/falling_sprites.h"
#include "game/main_menu.h"
#include "game/menu_button.h"
#include "game/task.h"

void EventScript_IsRunning(void);
void MainMenu_CycleColourUpdate(void);
void MainMenu_DrawFrame(void);
void MainMenu_FlipToOtherSide(void);
void MainMenu_HandleKeys(void);
void MainMenu_InitFallingSprites(void);
void MainMenu_LoadPageColour(void);
void MainMenu_OnDestroy(void);
void MainMenu_OpenUpdate(void);
void MainMenu_PageFlipUpdate(void);
void MainMenu_Setup(void);
void MedalCountTask_Update(void);
extern TaskScriptCmd data_02159b44[];
void func_0202dd90(void);
void func_0202df78(void);
void func_0202ea08(void);
void func_0202eccc(void);
void func_0202ef24(void);
void func_0202f94c(void);
void CursorBandTask_Update(void);
void MainMenu_ConfirmEntry(void);
void MainMenu_UpdateFallingSprites(void);
void MainMenu_CancelEntry(void);

MainMenu* data_021599d0 = &data_0229c720;
TaskScriptCmd data_021599d4[] = {
#include "data/main/data_021599d4.inc.c"
};
FallingSpriteFrame data_021599f8[] = {
#include "data/main/data_021599f8.inc.c"
};
FallingSpriteFrame data_021599e4[] = {
#include "data/main/data_021599e4.inc.c"
};
TaskScriptCmd data_02159a0c[] = {
#include "data/main/data_02159a0c.inc.c"
};
TaskScriptCmd data_02159a24[] = {
#include "data/main/data_02159a24.inc.c"
};
TaskScriptCmd data_02159a7c[] = {
#include "data/main/data_02159a7c.inc.c"
};
TaskScriptCmd data_02159a3c[] = {
#include "data/main/data_02159a3c.inc.c"
};
TaskScriptCmd data_02159a5c[] = {
#include "data/main/data_02159a5c.inc.c"
};
MenuButtonTemplate data_02159ac4 = {
#include "data/main/data_02159ac4.inc.c"
};
MenuButtonTemplate data_02159aec = {
#include "data/main/data_02159aec.inc.c"
};
MenuButtonTemplate data_02159a9c = {
#include "data/main/data_02159a9c.inc.c"
};
TaskScriptCmd data_02159b14[] = {
#include "data/main/data_02159b14.inc.c"
};
TaskScriptCmd data_02159b44[] = {
#include "data/main/data_02159b44.inc.c"
};

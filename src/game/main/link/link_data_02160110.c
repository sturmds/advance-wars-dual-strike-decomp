#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void HelpBand_Stop(void);
void LinkMapRowSync_Update(void);
void LinkMarkerTask_GrowIn(void);
void LinkMarkerTask_SquashOut(void);
void LinkMenuTask_HandlePeerMessages(void);
void LinkMenuTask_HandlePicks(void);
void LinkMenu_DestroyMarkers(void);
void LinkMenu_DestroySubScreenTask(void);
void LinkMenu_DrawPlayersAndMaps(void);
void LinkMenu_HandleBack(void);
void LinkMenu_InitTwoPlayerScreen(void);
void LinkMenu_OnKeysStage1(void);
void LinkMenu_OnKeysStage2(void);
void LinkMenu_RequireMap(void);
void LinkMenu_SaveReceivedMap(void);
void LinkPickedMapSync_Update(void);
void LinkTask_WaitForReady(void);
void Link_DrawEntryMarkers(void);
void LoadingScreen_Init(void);
void Screen1_DestroyTextTasks(void);
void Wallpaper_DestroyTasks(void);
extern TaskScriptCmd data_021602d0[];
void func_0207b4e8(void);
void func_0207bfb8(void);
void func_0207c018(void);
void func_0207e5e4(void);
void func_0207e5fc(void);
void func_0207e62c(void);
void func_0207eb2c(void);
void func_0207edec(void);
void func_0207ee08(void);
void func_0207eec8(void);
void func_0207ef28(void);
void func_0207f544(void);
void func_0207fee0(void);
void func_02080098(void);
void func_020828c0(void);
void func_020b983c(void);
void func_0207e484(void);
void func_0207e5bc(void);
void LinkMenu_PreviewMapSlot(void);
void func_0207e4fc(void);

char data_02160110[] = {
#include "data/main/data_02160110.inc.c"
};
TaskScriptCmd data_02160118[] = {
#include "data/main/data_02160118.inc.c"
};
MenuButtonTemplate data_02160178 = {
#include "data/main/data_02160178.inc.c"
};
MenuButtonTemplate data_02160150 = {
#include "data/main/data_02160150.inc.c"
};
MenuButtonTemplate data_02160128 = {
#include "data/main/data_02160128.inc.c"
};
TaskScriptCmd data_021601a0[] = {
#include "data/main/data_021601a0.inc.c"
};
TaskScriptCmd gLinkMarkerScript1[] = {
#include "data/main/gLinkMarkerScript1.inc.c"
};
TaskScriptCmd gLinkMarkerScript2[] = {
#include "data/main/gLinkMarkerScript2.inc.c"
};
TaskScriptCmd gLinkMarkerScript0[] = {
#include "data/main/gLinkMarkerScript0.inc.c"
};
TaskScriptCmd data_02160278[] = {
#include "data/main/data_02160278.inc.c"
};
TaskScriptCmd data_021602d0[] = {
#include "data/main/data_021602d0.inc.c"
};

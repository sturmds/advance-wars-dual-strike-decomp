#include "types.h"
#include "game/event_script.h"
#include "game/menu_button.h"
#include "game/menu_help.h"
#include "game/menu_item.h"
#include "game/task.h"

void BattleMenu_AutoCoOffHidden(void);
void BattleMenu_AutoCoOnHidden(void);
void BattleMenu_DrawAiStrikeHelp(void);
void BattleMenu_DrawMusicOffHelp(void);
void BattleMenu_MusicOffHidden(void);
void BattleMenu_MusicOnHidden(void);
void BattleMenu_NoUnitsDisabled(void);
void BattleMenu_NotVersusHidden(void);
void BattleMenu_SwapCoHidden(void);
void BattleMenu_SwapCoTagFrontHidden(void);
void BuildMenu_OnKeys(void);
void BuildMenu_Setup(void);
void BuildMenu_SnapSidePanelLeft(void);
void Map_HasNoHintScript(void);
void Map_HasNoTermsScript(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void PlayerInfo_StartTaskForCurrentPlayer(void);
void PowerMenu_PickNextWeatherHidden(void);
void PowerMenu_TagHidden(void);
void UnitMenu_BoardHidden(void);
void UnitMenu_CaptureHidden(void);
void UnitMenu_ExplodeHidden(void);
void UnitMenu_FireBlockedHidden(void);
void UnitMenu_FireHidden(void);
void UnitMenu_JoinHidden(void);
void UnitMenu_OoziumHidden(void);
void UnitMenu_RepairHidden(void);
void UnitMenu_SamiCaptureHidden(void);
void UnitMenu_SendBlockedHidden(void);
void UnitMenu_SendHidden(void);
void UnitMenu_SupplyHidden(void);
void UnitMenu_WaitHidden(void);
void func_0200a5f8(void);
void func_0200a614(void);
void func_0200a630(void);
void func_0201ecf0(void);
void func_020647f8(void);
void func_020bba60(void);
void func_020bc960(void);
void func_020bdf48(void);
void func_020be150(void);
void func_020be358(void);
void func_020be560(void);
void func_020beda8(void);
void func_020bee00(void);
void func_020beed8(void);
void func_020bef30(void);
void func_020bf1f0(void);
void func_020bf260(void);
void func_020bf6f4(void);
void func_020bfbe0(void);
void func_020bfc44(void);
void func_020bfed4(void);
void func_020bff48(void);
void func_020bffbc(void);
void func_020c0030(void);
void func_020c00a4(void);
void func_020c0118(void);
void func_020c0144(void);
void func_020c014c(void);
void func_020c0178(void);
void func_020c01c0(void);
void func_020d33f0(void);
void func_020d340c(void);
void BattleMenu_DrawAiAssaultHelp(void);
void BattleMenu_DrawAiDefenseHelp(void);
void BattleMenu_DrawAiGeneralHelp(void);
void BattleMenu_DrawAutoCoOffHelp(void);
void BattleMenu_DrawAutoCoOnHelp(void);
void BattleMenu_DrawHelpByGameMode(void);
void BattleMenu_DrawMusicOnHelp(void);
void BuildMenu_OnConfirm(void);
void Menu_CycleAiBehaviour(void);
void Menu_CycleAnimationMode(void);
void Menu_ToggleMusic(void);
void UnitMenu_Board(void);
void UnitMenu_Capture(void);
void UnitMenu_Dive(void);
void UnitMenu_Emerge(void);
void UnitMenu_Fire(void);
void UnitMenu_Join(void);
void UnitMenu_SendShowTarget(void);
void UnitMenu_Wait(void);
void func_020bc75c(void);
void func_020bc770(void);
void func_020bc784(void);
void func_020bc798(void);
void func_020bc7ac(void);
void func_020bc7c0(void);
void func_020bc7d4(void);
void func_020bc7e8(void);
void func_020bc7fc(void);
void func_020bc810(void);
void func_020bc824(void);
void func_020bc838(void);
void func_020bc84c(void);
void func_020bc860(void);
void func_020bc874(void);
void func_020bc888(void);
void func_020bc89c(void);
void func_020bc8b0(void);
void func_020bc94c(void);
void func_020bc974(void);
void func_020bc988(void);
void func_020bc99c(void);
void func_020bc9b0(void);
void func_020bcf28(void);
void func_020bcfe0(void);
void func_020bd13c(void);
void func_020bd16c(void);
void func_020bd1bc(void);
void func_020bd1dc(void);
void func_020bd358(void);
void BattleMenu_OpenTaskScreen(void);
void BuildMenu_OnCancel(void);
void Map_StartHintScript(void);
void Map_StartTermsScript(void);
void Menu_PickNextWeather(void);
void Menu_SwapCo(void);
void Menu_ToggleAutoCo(void);
void PowerMenu_ActivateCo(void);
void PowerMenu_ActivateSuper(void);
void PowerMenu_ActivateTag(void);
void UnitMenu_Send(void);
void func_0201ecac(void);
void func_0201ece4(void);
void func_020bd50c(void);
void func_020bd538(void);
void func_020bd564(void);
void func_020bd5f4(void);
void func_020bd8c0(void);
void func_020bd900(void);
void func_020bde18(void);
void func_020bdeb4(void);
void func_020bdf08(void);
void func_020bdf28(void);
void func_020bfd50(void);
void func_020c02b0(void);
void func_020c02f0(void);
void BuildMenu_OnSelect(void);
void func_020c0330(void);

MenuHelpPair gMusicMenuHelpFns = {{(MenuHelpFn)&BattleMenu_DrawMusicOffHelp,
                                   (MenuHelpFn)&BattleMenu_DrawMusicOnHelp}};
u16 data_021658c8[] = {
#include "data/main/data_021658c8.inc.c"
};
MenuHelpFour gAiBehaviourMenuHelpFns = {
    {(MenuHelpFn)&BattleMenu_DrawAiStrikeHelp,
     (MenuHelpFn)&BattleMenu_DrawAiAssaultHelp,
     (MenuHelpFn)&BattleMenu_DrawAiGeneralHelp,
     (MenuHelpFn)&BattleMenu_DrawAiDefenseHelp}};
MenuHelpFive gAnimationMenuHelpFns = {
    {(MenuHelpFn)&func_020bc960, (MenuHelpFn)&func_020bc9b0,
     (MenuHelpFn)&func_020bc99c, (MenuHelpFn)&func_020bc988,
     (MenuHelpFn)&func_020bc974}};
MenuButtonTemplate gBuildMenuRowButton = {
#include "data/main/gBuildMenuRowButton.inc.c"
};
MenuButtonTemplate gNextTurnButton = {
#include "data/main/gNextTurnButton.inc.c"
};
TaskScriptCmd data_0216594c[] = {
#include "data/main/data_0216594c.inc.c"
};
EventScriptCommand gSendNoSlotScript[] = {
#include "data/main/gSendNoSlotScript.inc.c"
};
EventScriptCommand gSendNoTileScript[] = {
#include "data/main/gSendNoTileScript.inc.c"
};
TaskScriptCmd data_02165a1c[] = {
#include "data/main/data_02165a1c.inc.c"
};
TaskScriptCmd data_02165a84[] = {
#include "data/main/data_02165a84.inc.c"
};
MenuItem data_02165aec[] = {
#include "data/main/data_02165aec.inc.c"
};
MenuItem data_02165d2c[] = {
#include "data/main/data_02165d2c.inc.c"
};
MenuItem data_02165bcc[] = {
#include "data/main/data_02165bcc.inc.c"
};
MenuItem gBattleMenuItems[] = {
#include "data/main/gBattleMenuItems.inc.c"
};
MenuItem gUnitMenuItems[] = {
#include "data/main/gUnitMenuItems.inc.c"
};

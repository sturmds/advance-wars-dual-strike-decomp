#include "types.h"

void EventScript_RunCommand(void);
void EventScript_StartWait(void);
void func_0201d5f8(void);
void func_0201d5f0(void);
void EventScript_StoreByte(void);
void EventScript_StoreHalfword(void);
void EventScript_StoreWord(void);
void EventScript_CopyByte(void);
void EventScript_CopyHalfword(void);
void EventScript_CopyWord(void);
void EventCmd_SetMapId(void);
void func_0201d614(void);
void func_0201d428(void);
void EventCmd_CreateUnit(void);
void EventCmd_WaitWithKeys(void);
void func_0201ce50(void);
void func_0201ce1c(void);
void EventCmd_SetViewTargetTile(void);
void EventCmd_AddToViewTargetTile(void);
void EventCmd_ShowPicture(void);
void EventCmd_StartScreenSlideIn(void);
void EventCmd_PrintWindowText(void);
void EventCmd_PrintText(void);
void EventCmd_SetHold(void);
void EventCmd_WaitForKeys(void);
void EventCmd_Jump(void);
void EventCmd_JumpIfTrue(void);
void EventCmd_SetPlayerController(void);
void EventCmd_SetMapFlags(void);
void EventCmd_SetSecondScreenMapUnk37(void);
void EventCmd_SetPlayerFrontCo(void);
void func_0201ba3c(void);
void func_0201ba18(void);
void func_0201b9f4(void);
void func_0201b9d0(void);
void func_0201b9c8(void);
void func_0201b9a4(void);
void EventCmd_StartHelperScript(void);
void EventCmd_StopHelperScript(void);
void EventCmd_WaitForAttackTasks(void);
void EventCmd_PrintSlotText(void);
void EventCmd_SetFlag84RunActionHook(void);
void EventCmd_ClearFlag84RunActionHook(void);
void EventCmd_LoadCurrentArmyPaletteSlot15(void);
void EventCmd_LoadCurrentArmyBgRow(void);
void EventCmd_LoadArmyPalette(void);
void EventScript_SetPalette(void);
void EventCmd_LoadCurrentArmyPalette(void);
void EventCmd_WaitForState14(void);
void EventCmd_WaitWhileState14(void);
void EventCmd_CallFunction(void);
void EventCmd_HoldUntilTurn(void);
void EventCmd_ChangePicture(void);
void EventCmd_SetPicture(void);
void EventCmd_StopOtherScripts(void);
void EventCmd_HoldUntilPower(void);
void func_0201b788(void);
void EventCmd_RollWeatherAndRefreshUnits(void);
void EventCmd_JumpIfFlagSet(void);
void EventCmd_JumpIfFlagClear(void);
void func_0201b5dc(void);
void func_0201b524(void);
void func_0201b46c(void);
void func_0201b3b4(void);
void func_0201b358(void);
void EventScript_PlaySe(void);
void EventScript_PlaySong(void);
void EventScript_PlayCurrentCoTheme(void);
void EventScript_FadeOutSongAfterRequest(void);
void EventScript_FadeOutSongHardMode(void);
void EventScript_FadeOutSong(void);
void EventScript_WaitSongFade(void);
void EventScript_SkipUnlessCo(void);
void EventScript_JumpIfCos(void);
void EventScript_SetFlag(void);
void EventScript_ClearFlag(void);
void EventScript_ScrollMapAndWait(void);
void func_0201d378(void);
void func_0201d320(void);
void EventScript_StartTask(void);
void EventScript_WaitTask(void);
void EventScript_WaitTaskScreen0(void);
void EventScript_WaitTaskScreen1(void);
void EventScript_WaitTaskBothScreens(void);
void EventScript_CallUnk4658(void);
void EventScript_SetTimeLimit(void);
void EventScript_ClearTimeLimit(void);
void EventCmd_StartTextMapTask(void);

/* declared `EventCmdFunc data_021585c0[]` in event_script_step_slot.c: declared in src/game/main/script/event_script_step_slot.c, with a type no header defines */
void* data_021585c0[] = {&EventScript_RunCommand,
                         &EventScript_StartWait,
                         &func_0201d5f8,
                         &func_0201d5f8,
                         &func_0201d5f8,
                         &func_0201d5f0,
                         &EventScript_StoreByte,
                         &EventScript_StoreHalfword,
                         &EventScript_StoreWord,
                         &EventScript_CopyByte,
                         &EventScript_CopyHalfword,
                         &EventScript_CopyWord,
                         &EventCmd_SetMapId,
                         &func_0201d614,
                         &EventCmd_SetMapId,
                         &func_0201d428,
                         &EventCmd_CreateUnit,
                         &EventCmd_WaitWithKeys,
                         &func_0201ce50,
                         &func_0201ce1c,
                         &EventCmd_SetViewTargetTile,
                         &EventCmd_AddToViewTargetTile,
                         &func_0201d5f8,
                         &EventCmd_ShowPicture,
                         &EventCmd_StartScreenSlideIn,
                         &EventCmd_PrintWindowText,
                         &EventCmd_PrintText,
                         &EventCmd_SetHold,
                         &EventCmd_WaitForKeys,
                         &EventCmd_Jump,
                         &EventCmd_JumpIfTrue,
                         &EventCmd_SetPlayerController,
                         &EventCmd_SetMapFlags,
                         &EventCmd_SetSecondScreenMapUnk37,
                         &EventCmd_SetPlayerFrontCo,
                         &func_0201ba3c,
                         &func_0201ba18,
                         &func_0201b9f4,
                         &func_0201b9d0,
                         &func_0201b9c8,
                         &func_0201b9a4,
                         &EventCmd_StartHelperScript,
                         &EventCmd_StopHelperScript,
                         &EventCmd_WaitForAttackTasks,
                         &EventCmd_PrintSlotText,
                         &EventCmd_SetFlag84RunActionHook,
                         &EventCmd_ClearFlag84RunActionHook,
                         &EventCmd_LoadCurrentArmyPaletteSlot15,
                         &EventCmd_LoadCurrentArmyBgRow,
                         &EventCmd_LoadArmyPalette,
                         &EventScript_SetPalette,
                         &EventCmd_LoadCurrentArmyPalette,
                         &EventCmd_WaitForState14,
                         &EventCmd_WaitWhileState14,
                         &EventCmd_CallFunction,
                         &EventCmd_HoldUntilTurn,
                         0,
                         &EventCmd_ChangePicture,
                         &EventCmd_SetPicture,
                         &EventCmd_StopOtherScripts,
                         &EventCmd_HoldUntilPower,
                         &func_0201b788,
                         &EventCmd_RollWeatherAndRefreshUnits,
                         &EventCmd_JumpIfFlagSet,
                         &EventCmd_JumpIfFlagClear,
                         &func_0201b5dc,
                         &func_0201b524,
                         &func_0201b46c,
                         &func_0201b3b4,
                         &func_0201b358,
                         &EventScript_PlaySe,
                         &EventScript_PlaySong,
                         &EventScript_PlayCurrentCoTheme,
                         &EventScript_FadeOutSongAfterRequest,
                         &EventScript_FadeOutSongHardMode,
                         &EventScript_FadeOutSong,
                         &EventScript_WaitSongFade,
                         &EventScript_SkipUnlessCo,
                         &EventScript_JumpIfCos,
                         &EventScript_SetFlag,
                         &EventScript_ClearFlag,
                         &EventScript_ScrollMapAndWait,
                         &func_0201d378,
                         &func_0201d320,
                         &EventScript_StartTask,
                         &EventScript_WaitTask,
                         &EventScript_WaitTaskScreen0,
                         &EventScript_WaitTaskScreen1,
                         &EventScript_WaitTaskBothScreens,
                         &EventScript_CallUnk4658,
                         &EventScript_SetTimeLimit,
                         &EventScript_ClearTimeLimit,
                         &EventCmd_StartTextMapTask};

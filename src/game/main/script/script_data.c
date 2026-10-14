#include "types.h"
#include "game/event_script.h"

s16 EventScript_RunCommand(s16 index);
BOOL EventScript_StartWait(s16 index);
int func_0201d5f8(void);
int func_0201d5f0(int playerIndex, int coSlot);
BOOL EventScript_StoreByte(s16 index);
BOOL EventScript_StoreHalfword(s16 index);
BOOL EventScript_StoreWord(s16 index);
BOOL EventScript_CopyByte(s16 index);
BOOL EventScript_CopyHalfword(s16 index);
BOOL EventScript_CopyWord(s16 index);
BOOL EventCmd_SetMapId(s16 index);
BOOL func_0201d614(s16 index);
BOOL func_0201d428(s16 index);
BOOL EventCmd_CreateUnit(s16 index);
s16 EventCmd_WaitWithKeys(s16 index);
BOOL func_0201ce50(s16 index);
BOOL func_0201ce1c(s16 index);
BOOL EventCmd_SetViewTargetTile(s16 index);
BOOL EventCmd_AddToViewTargetTile(s16 index);
BOOL EventCmd_ShowPicture(s16 index);
BOOL EventCmd_StartScreenSlideIn(s16 index);
s16 EventCmd_PrintWindowText(int index);
s16 EventCmd_PrintText(int index);
BOOL EventCmd_SetHold(s16 index);
BOOL EventCmd_WaitForKeys(s16 index);
BOOL EventCmd_Jump(s16 index);
BOOL EventCmd_JumpIfTrue(s16 index);
BOOL EventCmd_SetPlayerController(s16 index);
BOOL EventCmd_SetMapFlags(s16 index);
BOOL EventCmd_SetSecondScreenMapUnk37(s16 index);
BOOL EventCmd_SetPlayerFrontCo(s16 index);
BOOL func_0201ba3c(s16 index);
BOOL func_0201ba18(s16 index);
BOOL func_0201b9f4(s16 index);
BOOL func_0201b9d0(s16 index);
int func_0201b9c8(int playerIndex, int coSlot);
BOOL func_0201b9a4(s16 index);
BOOL EventCmd_StartHelperScript(s16 index);
BOOL EventCmd_StopHelperScript(s16 index);
BOOL EventCmd_WaitForAttackTasks(s16 index);
BOOL EventCmd_PrintSlotText(s16 index);
BOOL EventCmd_SetFlag84RunActionHook(s16 index);
BOOL EventCmd_ClearFlag84RunActionHook(s16 index);
BOOL EventCmd_LoadCurrentArmyPaletteSlot15(s16 index);
BOOL EventCmd_LoadCurrentArmyBgRow(s16 index);
BOOL EventCmd_LoadArmyPalette(s16 index);
BOOL EventScript_SetPalette(s16 index);
BOOL EventCmd_LoadCurrentArmyPalette(s16 index);
BOOL EventCmd_WaitForState14(s16 index);
BOOL EventCmd_WaitWhileState14(s16 index);
s16 EventCmd_CallFunction(s16 index);
BOOL EventCmd_HoldUntilTurn(s16 index);
s16 EventCmd_ChangePicture(s16 index);
BOOL EventCmd_SetPicture(s16 index);
BOOL EventCmd_StopOtherScripts(s16 index);
BOOL EventCmd_HoldUntilPower(s16 index);
BOOL func_0201b788(s16 index);
BOOL EventCmd_RollWeatherAndRefreshUnits(s16 index);
BOOL EventCmd_JumpIfFlagSet(s16 index);
BOOL EventCmd_JumpIfFlagClear(s16 index);
BOOL func_0201b5dc(s16 index);
BOOL func_0201b524(s16 index);
BOOL func_0201b46c(s16 index);
BOOL func_0201b3b4(s16 index);
s16 func_0201b358(s16 index);
BOOL EventScript_PlaySe(s16 index);
s16 EventScript_PlaySong(s16 index);
s16 EventScript_PlayCurrentCoTheme(s16 index);
BOOL EventScript_FadeOutSongAfterRequest(s16 index);
BOOL EventScript_FadeOutSongHardMode(s16 index);
BOOL EventScript_FadeOutSong(s16 index);
BOOL EventScript_WaitSongFade(s16 index);
BOOL EventScript_SkipUnlessCo(int index);
BOOL EventScript_JumpIfCos(s16 index);
BOOL EventScript_SetFlag(s16 index);
BOOL EventScript_ClearFlag(s16 index);
BOOL EventScript_ScrollMapAndWait(s16 index);
BOOL func_0201d378(s16 index);
BOOL func_0201d320(s16 index);
BOOL EventScript_StartTask(s16 index);
BOOL EventScript_WaitTask(s16 index);
BOOL EventScript_WaitTaskScreen0(s16 index);
BOOL EventScript_WaitTaskScreen1(s16 index);
BOOL EventScript_WaitTaskBothScreens(s16 index);
BOOL EventScript_CallUnk4658(s16 index);
BOOL EventScript_SetTimeLimit(s16 index);
BOOL EventScript_ClearTimeLimit(s16 index);
BOOL EventCmd_StartTextMapTask(s16 index);

EventCmdFunc data_021585c0[] = {(EventCmdFunc)&EventScript_RunCommand,
                                &EventScript_StartWait,
                                (EventCmdFunc)&func_0201d5f8,
                                (EventCmdFunc)&func_0201d5f8,
                                (EventCmdFunc)&func_0201d5f8,
                                (EventCmdFunc)&func_0201d5f0,
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
                                (EventCmdFunc)&EventCmd_WaitWithKeys,
                                &func_0201ce50,
                                &func_0201ce1c,
                                &EventCmd_SetViewTargetTile,
                                &EventCmd_AddToViewTargetTile,
                                (EventCmdFunc)&func_0201d5f8,
                                &EventCmd_ShowPicture,
                                &EventCmd_StartScreenSlideIn,
                                (EventCmdFunc)&EventCmd_PrintWindowText,
                                (EventCmdFunc)&EventCmd_PrintText,
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
                                (EventCmdFunc)&func_0201b9c8,
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
                                (EventCmdFunc)&EventCmd_CallFunction,
                                &EventCmd_HoldUntilTurn,
                                0,
                                (EventCmdFunc)&EventCmd_ChangePicture,
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
                                (EventCmdFunc)&func_0201b358,
                                &EventScript_PlaySe,
                                (EventCmdFunc)&EventScript_PlaySong,
                                (EventCmdFunc)&EventScript_PlayCurrentCoTheme,
                                &EventScript_FadeOutSongAfterRequest,
                                &EventScript_FadeOutSongHardMode,
                                &EventScript_FadeOutSong,
                                &EventScript_WaitSongFade,
                                (EventCmdFunc)&EventScript_SkipUnlessCo,
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

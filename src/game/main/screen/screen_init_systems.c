#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int data_0229eae4;

void Screen_FillBlack(void);
void MainLoop_SetFrameCallback(void (*callback)(void));
void func_01ff9c28(void (*callback)(void));
void func_01ff9f1c(int value);
void func_01ff9f8c(int value);
void func_01ff9f54(int value);
void func_02020860(void);
void func_020bb02c(void);
void Game_ResetSettings(void);
void func_020032a8(void);
void func_02017520(void);
void TransferQueue_Clear(void);
void CallbackSlots_Clear(void);
void CallbackList_Clear(void);
void EventScript_Init(void);
void ScriptSlot_ClearAll(void);
void Task_InitSystem(void);
void UnitShowTask_ClearSlots(void);
void Screens_InitEffectState(void);
void Keypad_Init(void);
void func_020044a8(void);
void Screen_ApplyAll(void);
void Sound_ResetSongs(void);
void func_02026540(void);
void MainMenu_Init(void);
void IconBanks_ClearSlots(void);

/* Resets the current screen's systems: main loop callbacks and flags, part
 * of the map state, tasks, script slots, transfer and callback queues,
 * keypad, screen registers and songs. main calls it once per screen. */
void Screen_InitSystems(void)
{
    data_0229eae4 = 0;
    Screen_FillBlack();
    MainLoop_SetFrameCallback(NULL);
    func_01ff9c28(NULL);
    func_01ff9f1c(0);
    func_01ff9f8c(0);
    func_01ff9f54(0);
    data_027c027c->unk_48[data_027c0284]->unk_48 = 0;
    data_027c027c->unk_48[data_027c0284]->unk_4c = 0;
    data_027c027c->unk_48[data_027c0284]->unk_50 = 0;
    data_027c027c->unk_48[data_027c0284]->unk_54 = 0;
    func_02020860();
    func_020bb02c();
    Game_ResetSettings();
    func_020032a8();
    func_02017520();
    TransferQueue_Clear();
    CallbackSlots_Clear();
    CallbackList_Clear();
    EventScript_Init();
    ScriptSlot_ClearAll();
    Task_InitSystem();
    UnitShowTask_ClearSlots();
    Screens_InitEffectState();
    Keypad_Init();
    func_020044a8();
    Screen_ApplyAll();
    Sound_ResetSongs();
    func_02026540();
    MainMenu_Init();
    IconBanks_ClearSlots();
}

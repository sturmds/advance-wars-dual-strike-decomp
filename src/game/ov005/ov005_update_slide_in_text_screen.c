#include "types.h"
#include "game/task.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/ov005_task.h"
#include "game/screen_other_map.h"
#include "game/text_printer.h"
#include "game/Unk_ov005_02350e2c.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
Task *func_020fcdc8(Ov005Task *parent, int frames);
Task *func_020fcda0(Ov005Task *parent, int frames);
Task *func_020fcd78(Ov005Task *parent, int frames);
Task *func_020fcd50(Ov005Task *parent, int frames);
BOOL func_020fd624(Task *task);
void func_020fd618(Task *task);
void func_020fd560(void);
void func_02015da8(TextPrinter *printer);
BOOL Text_IsPrinting(void);
void Text_StopPrinting(void);
void Task_Destroy(Task *task);
void Task_ResumeScript(Ov005Task *task);

extern const TaskScriptCmd data_ov005_02368048[];
extern const MenuButtonTemplate data_ov005_02368070;

/* Per-frame update. Every frame it eases the x scroll of layers 1 and 2 on
 * screen data_027c0288.unk_04 (0 to 120 and 256 to 376 pixels). Steps: start
 * the child task and fade in (64 frames); wait until the text is printed, or
 * on A call func_02015da8 on the printer and go on; add button 253 and wait
 * for A or the button; fade out, hide all layers on both screens, remove the
 * button, the child task and the text, and resume the task script. */
void Ov005_UpdateSlideInTextScreen(Ov005Task *task)
{
    Unk_ov005_02350e2c *work = task->work;
    TextPrinter *printer = work->printer;
    Unk_027c0280 *savedGfx = data_027c0280;
    Unk_02080440 *savedScreen;
    int x1;
    int x2;
    Task *child;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    x1 = Math_Ease(0, 0, 0x78000, work->frame, work->frames);
    x2 = Math_Ease(0, 0x100000, 0x178000, work->frame, work->frames);
    if (work->frame < work->frames) {
        work->frame++;
    }
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = x1 >> 12;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = x2 >> 12;
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);

    switch (task->step) {
    case 0:
        child = Task_Create(data_ov005_02368048, task);
        ((Ov005Task *)child)->work = work;
        work->childTask = child;
        work->unk_28 = 0;
        work->fadeTask = func_020fcdc8(task, 64);
        work->fadeTask2 = func_020fcda0(task, 64);
        task->step++;
    case 1:
        if (func_020fd624(work->fadeTask)) {
            return;
        }
        if (func_020fd624(work->fadeTask2)) {
            return;
        }
        func_020fd618(work->fadeTask);
        func_020fd618(work->fadeTask2);
        task->step = 10;
        return;
    case 10:
        if (gKeypad->pressed & PAD_BUTTON_A) {
            func_02015da8(printer);
            task->step++;
            return;
        }
        if (!Text_IsPrinting()) {
            task->step++;
        }
        return;
    case 11:
        MenuButton_Add(0, 0, &data_ov005_02368070, 253);
        MenuButton_Select(253);
        work->buttonPressed = FALSE;
        task->step++;
    case 12:
        if (gKeypad->pressed & PAD_BUTTON_A) {
            task->step = 100;
            return;
        }
        if (work->buttonPressed) {
            task->step = 100;
        }
        return;
    case 100:
        work->fadeTask = func_020fcd78(task, 64);
        work->fadeTask2 = func_020fcd50(task, 64);
        task->step++;
        return;
    case 101:
        if (func_020fd624(work->fadeTask)) {
            return;
        }
        if (func_020fd624(work->fadeTask2)) {
            return;
        }
        func_020fd560();
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        savedScreen = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        GFX_DISPCNT(data_027c0280)->bg0 = 0;
        GFX_DISPCNT(data_027c0280)->bg1 = 0;
        GFX_DISPCNT(data_027c0280)->bg2 = 0;
        GFX_DISPCNT(data_027c0280)->bg3 = 0;
        GFX_DISPCNT(data_027c0280)->obj = 0;
        Screen_SetCurrent(savedScreen);
        Screen_SetCurrentGfx(savedGfx);
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedScreen = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        GFX_DISPCNT(data_027c0280)->bg0 = 0;
        GFX_DISPCNT(data_027c0280)->bg1 = 0;
        GFX_DISPCNT(data_027c0280)->bg2 = 0;
        GFX_DISPCNT(data_027c0280)->bg3 = 0;
        GFX_DISPCNT(data_027c0280)->obj = 0;
        Screen_SetCurrent(savedScreen);
        Screen_SetCurrentGfx(savedGfx);
        func_020fd618(work->fadeTask);
        func_020fd618(work->fadeTask2);
        MenuButton_Remove(253);
        Task_Destroy(work->childTask);
        Text_StopPrinting();
        task->step = 102;
        return;
    case 102:
        Task_ResumeScript(task);
        return;
    }
}

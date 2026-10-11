#include "types.h"
#include "sdk/gx.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0235a30c.h"
#include "game/brightness_fade.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
BOOL func_020fd624(Task *task);
void func_020fd618(Task *task);
void Screens_BlackOut(void);
void DmaTask_Start(const void *src, volatile void *dest, int unk2, int unk3, void (*callback)(void));
void DmaTasks_Destroy(void);
void Sound_StopStream(int fadeFrames);
void Task_ResumeScript(Ov005Task *task);
void *Ov005_LoadCreditsPage(Unk_ov005_0235a30c *work, int page);
void Ov005_CreateEntryTasks(Unk_ov005_0235a30c *work);
void Ov005_DrawCreditsEntryText(Unk_ov005_0235a30c *work);
void Ov005_InitCenteredEntries(Unk_ov005_0235a30c *work);
BOOL func_ov005_0235ad48(Unk_ov005_0235a30c *work);
BOOL func_ov005_0235ad30(Unk_ov005_0235a30c *work);
void func_ov005_0235abe4(Unk_ov005_0235a30c *work);
void func_ov005_0235abcc(Unk_ov005_0235a30c *work);
void Ov005Slide_StartAll(Unk_ov005_0235a30c *work);
void func_ov005_0235a30c(Unk_ov005_0235a30c *work);
void Ov005_SkipOnInput(Unk_ov005_0235a30c *work, Ov005Task *task);
void Ov005_DrawCreditsEntries3D(Unk_ov005_0235a30c *work);
void Ov005_QueueEntrySprites(Unk_ov005_0235a30c *work);
void func_ov005_0235a320(Unk_ov005_0235a30c *work);
void func_ov005_0235bab4(void);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const u16 data_ov005_0236970c[];

/* Per-frame update of the staff credits. Fades both screens in, moves the
 * sub screen's BG layer 2 from y = -48 to 0 while alpha blending it in, then
 * shows the pages of data_ov005_0236a418 ("PRODUCER", "DIRECTOR", ...) one at
 * a time: the page's lines get entry tasks, stay for the page's frame count,
 * and are copied centred into the second entry table. After the last page
 * both screens fade out with their layers hidden, the stream stops and the
 * task script resumes. A touch or A/Start skips to the fade out
 * (Ov005_SkipOnInput). */
void Ov005_UpdateStaffCreditsScreen(Ov005Task *task)
{
    Unk_ov005_0235a30c *work = task->work;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedScreen;

    switch (task->step) {
    case 0:
        work->fadeTask = func_020fcdc8(task, 64);
        work->fadeTask2 = func_020fcda0(task, 64);
        task->step++;
    case 1:
        if (func_020fd624(work->fadeTask)) {
            break;
        }
        if (func_020fd624(work->fadeTask2)) {
            break;
        }
        func_020fd618(work->fadeTask);
        func_020fd618(work->fadeTask2);
        task->step = 10;
        break;
    case 10:
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        savedScreen = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        data_027c0280->bgScrollY[data_027c0280->layer2Bg] = -48;
        Screen_SetCurrent(savedScreen);
        Screen_SetCurrentGfx(savedGfx);
        task->frame = 0;
        task->frames = 64;
        task->step++;
    case 11: {
        /* needed to match: this block gets its own locals */
        Unk_027c0280 *savedGfx11;
        Unk_02080440 *savedScreen11;
        int value;
        u8 bg;

        savedGfx11 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        savedScreen11 = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        bg = data_027c0280->layer2Bg;
        data_027c0280->bgScrollY[bg] = Math_Ease(4, -48, 0, task->frame, task->frames);
        value = Math_Ease(0, 0, 16, task->frame, task->frames);
        data_027c0280->blendMode = BLEND_MODE_ALPHA;
        data_027c0280->blendEva = value;
        data_027c0280->blendEvb = 16 - value;
        data_027c0280->blendEvy = 0;
        Screen_SetCurrent(savedScreen11);
        Screen_SetCurrentGfx(savedGfx11);
    }
        if (task->frame <= task->frames) {
            task->frame++;
        }
        if (task->frame > task->frames) {
            savedGfx = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_04);
            savedScreen = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_04);
            data_027c0280->blendMode = BLEND_MODE_ALPHA;
            data_027c0280->blendEva = 8;
            data_027c0280->blendEvb = 8;
            data_027c0280->blendEvy = 0;
            data_027c0280->blendTopBackdrop = 0;
            data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
            data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
            data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
            data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
            data_027c0280->blendTopObj = 1;
            data_027c0280->blendBottomBackdrop = 0;
            data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
            data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
            data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
            data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
            data_027c0280->blendBottomObj = 0;
            DmaTask_Start(data_ov005_0236970c, REG_DB_BLDALPHA, 0, 0, func_ov005_0235bab4);
            Screen_SetCurrent(savedScreen);
            Screen_SetCurrentGfx(savedGfx);
            task->step = 20;
        }
        break;
    case 20:
        if (Ov005_LoadCreditsPage(work, work->pageIndex) == NULL) {
            task->step = 90;
            break;
        }
        task->step = 60;
    case 60:
        Ov005_CreateEntryTasks(work);
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedScreen = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        Ov005_DrawCreditsEntryText(work);
        Screen_SetCurrent(savedScreen);
        Screen_SetCurrentGfx(savedGfx);
        task->frame = 0;
        task->frames = work->pageFrames;
        task->step++;
    case 61:
        if (func_ov005_0235ad48(work)) {
            break;
        }
        func_ov005_0235abe4(work);
        task->step++;
        break;
    case 62:
        if (task->frame <= task->frames) {
            task->frame++;
        }
        if (task->frame > task->frames) {
            if (func_ov005_0235ad30(work)) {
                break;
            }
            func_ov005_0235abcc(work);
            task->step = 70;
        }
        break;
    case 70:
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        savedScreen = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        Ov005_InitCenteredEntries(work);
        Ov005_DrawCreditsEntryText(work);
        Screen_SetCurrent(savedScreen);
        Screen_SetCurrentGfx(savedGfx);
        task->frame = 0;
        task->frames = 32;
        task->step++;
    case 71:
        if (task->frame <= task->frames) {
            task->frame++;
        }
        if (task->frame > task->frames) {
            task->step = 80;
        }
        break;
    case 80:
        Ov005Slide_StartAll(work);
        task->step++;
    case 81:
        if (func_ov005_0235ad48(work)) {
            break;
        }
        func_ov005_0235abe4(work);
        task->step = 82;
        break;
    case 82:
        work->pageIndex++;
        task->step = 20;
        break;
    case 90:
        if (func_ov005_0235ad30(work)) {
            break;
        }
        func_ov005_0235abcc(work);
        task->step = 100;
        break;
    case 100:
        work->fadeTask = BrightnessFade_StartOutBlackScreen0(task, 64);
        work->fadeTask2 = func_020fcd50(task, 64);
        task->step++;
        break;
    case 101:
        if (func_020fd624(work->fadeTask)) {
            break;
        }
        if (func_020fd624(work->fadeTask2)) {
            break;
        }
        func_ov005_0235abe4(work);
        func_ov005_0235abcc(work);
        func_ov005_0235a30c(work);
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        savedScreen = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        DmaTasks_Destroy();
        Screen_SetCurrent(savedScreen);
        Screen_SetCurrentGfx(savedGfx);
        Screens_BlackOut();
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        savedScreen = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        data_027c0280->dispCnt &= ~0x100;
        data_027c0280->dispCnt &= ~0x200;
        data_027c0280->dispCnt &= ~0x400;
        data_027c0280->dispCnt &= ~0x800;
        data_027c0280->dispCnt &= ~0x1000;
        Screen_SetCurrent(savedScreen);
        Screen_SetCurrentGfx(savedGfx);
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedScreen = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        data_027c0280->dispCnt &= ~0x100;
        data_027c0280->dispCnt &= ~0x200;
        data_027c0280->dispCnt &= ~0x400;
        data_027c0280->dispCnt &= ~0x800;
        data_027c0280->dispCnt &= ~0x1000;
        Screen_SetCurrent(savedScreen);
        Screen_SetCurrentGfx(savedGfx);
        func_020fd618(work->fadeTask);
        func_020fd618(work->fadeTask2);
        task->step++;
        break;
    case 102:
        Sound_StopStream(32);
        task->frame = 0;
        task->frames = 32;
        task->step++;
    case 103:
        if (task->frame <= task->frames) {
            task->frame++;
        }
        if (task->frame > task->frames) {
            Task_ResumeScript(task);
        }
        break;
    }
    Ov005_SkipOnInput(work, task);
    Ov005_DrawCreditsEntries3D(work);
    Ov005_QueueEntrySprites(work);
    func_ov005_0235a320(work);
}

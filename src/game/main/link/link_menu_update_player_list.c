#include "types.h"
#include "game/link_wait_task.h"
#include "game/link_menu_state.h"

int func_0207a27c(void);
int func_0207a120(void);
int func_0207a0f8(void);
int func_0207a0d0(void);
void func_0207cec4(LinkWaitTask *task);
void func_0207ceb8(LinkWaitTask *task);
void func_0207ceac(LinkWaitTask *task);
void LinkMenu_UpdateParent(LinkWaitTask *task);
void LinkMenu_UpdateChild(LinkWaitTask *task);
void LinkMenu_HandlePressed(void);
void func_0207c044(LinkWaitTask *task, u32 textId);
void func_0207c380(void);
void LinkMenu_RestoreRowButtons(void);

/* Task step of the player list screen. Unlocks the screen, runs the step of
 * the current link state (4 parent, 5 child), then handles A and B. Each of
 * three one-shot link events (func_0207a120, func_0207a0f8, func_0207a0d0)
 * shows a banner with its message (text 0x48000000-0x48000002); all but the
 * second on the second view also remove the data_0215fee4 banner and restore
 * the row buttons. */
void LinkMenu_UpdatePlayerList(LinkWaitTask *task)
{
    data_0215fed0->locked = 0;
    switch (func_0207a27c()) {
    case 0:
        func_0207cec4(task);
        break;
    case 3:
        func_0207ceb8(task);
        break;
    case 2:
        func_0207ceac(task);
        break;
    case 4:
        LinkMenu_UpdateParent(task);
        break;
    case 5:
        LinkMenu_UpdateChild(task);
        break;
    }
    LinkMenu_HandlePressed();
    task->waitFrames++;
    if (func_0207a120() == 1) {
        func_0207c044(task, 0x48000000);
        func_0207c380();
        LinkMenu_RestoreRowButtons();
    }
    if (func_0207a0f8() == 1) {
        if (data_0215fed0->view == 0) {
            func_0207c044(task, 0x48000000);
            func_0207c380();
            LinkMenu_RestoreRowButtons();
        } else {
            func_0207c044(task, 0x48000001);
        }
    }
    if (func_0207a0d0() == 1) {
        func_0207c044(task, 0x48000002);
        func_0207c380();
        LinkMenu_RestoreRowButtons();
    }
}

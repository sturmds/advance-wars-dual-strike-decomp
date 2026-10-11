#include "types.h"
#include "game/map_select.h"
#include "game/event_queue.h"
#include "game/keypad.h"
#include "game/task.h"

u8 *func_02078138(int player);
void func_0207b48c(void);
u32 Link_IsActive(void);
int func_0207bd94(void);
void func_0205ec08(int unused);
void func_0205ec14(int unused);
void func_0205d56c(void);
void MapSelect_PrevList(void);
void MapSelect_NextList(void);
void MapSelect_TurnToPage(u32 page);
void MapSelect_SelectPagePosition(int pos);
void MapSelect_ConfirmMap(void);
void MapSelect_PickFirstMap(void);
void MapSelect_CancelFirstMap(void);
void Task_ResumeScript(Task *task);

extern MenuStateFrom400 data_0229ef30;

/* Task step of the map select menu. In the War Room, left and right turn the
 * strip of MapSelect_PrevSubStrip and MapSelect_NextSubStrip. In link play
 * every machine then applies the messages from the event queue (up to four,
 * stopping during a list slide), so all screens change together; confirm or
 * leave ends the menu and resumes the task's script. A confirm in a two-map
 * battle before the first map is picked does nothing. */
void MapSelect_Update(Task *task)
{
    int confirm;
    int leave;
    u8 *msg;
    int i;

    confirm = 0;
    MENU->setup.listChanged = 0;
    leave = 0;
    if (STATE->gameMode == GAME_MODE_WAR_ROOM && MENU->setup.subSlideFrames == 0) {
        if (gKeypad->repeat & PAD_KEY_LEFT) {
            func_0205ec14(0);
        } else if (gKeypad->repeat & PAD_KEY_RIGHT) {
            func_0205ec08(0);
        }
    }
    if (Link_IsActive() != 1) {
        return;
    }
    if (func_0207bd94() == 1) {
        msg = func_02078138(0);
        for (i = 0; i < 4; i++, msg += 2) {
            if (MENU->setup.slideFrames != 0 || confirm == TRUE || leave == TRUE) {
                break;
            }
            switch (msg[0]) {
            case EVENT_MAP_SELECT_PREV_LIST:
                MapSelect_PrevList();
                break;
            case EVENT_MAP_SELECT_NEXT_LIST:
                MapSelect_NextList();
                break;
            case EVENT_MAP_SELECT_PAGE:
                MapSelect_TurnToPage(msg[1]);
                break;
            case EVENT_MAP_SELECT_POSITION:
                MapSelect_SelectPagePosition(msg[1]);
                break;
            case EVENT_MAP_SELECT_CONFIRM:
                if (STATE->twoMaps == 1 && data_0229ef30.unk_4fc == 0) {
                    break;
                }
                confirm = TRUE;
                MapSelect_SelectPagePosition(msg[1]);
                break;
            case EVENT_MAP_SELECT_LEAVE:
                leave = TRUE;
                break;
            case EVENT_MAP_SELECT_PICK_FIRST_MAP:
                MapSelect_SelectPagePosition(msg[1]);
                MapSelect_PickFirstMap();
                break;
            case EVENT_MAP_SELECT_CANCEL_FIRST_MAP:
                MapSelect_CancelFirstMap();
                break;
            }
        }
        EventQueue_Drain();
    }
    if (confirm == TRUE) {
        MapSelect_ConfirmMap();
        EventQueue_Reset();
        func_0207b48c();
        Task_ResumeScript(task);
        return;
    }
    if (leave == TRUE) {
        func_0205d56c();
        EventQueue_Reset();
        func_0207b48c();
        Task_ResumeScript(task);
    }
}

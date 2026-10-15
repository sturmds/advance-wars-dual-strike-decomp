#include "types.h"
#include "game/task.h"
#include "game/link_menu_state.h"
#include "game/menu_button.h"

#define LINK_UNFLAGGED_ACK 0x8f
#define SE_CONFIRM 0x10006
#define SE_CANCEL 0x10009

void Sound_PlaySe(u32 soundId);
int Wireless_GetState(void);
int func_0207a170(void);
u16 Wireless_GetChildAids(void);
BOOL Wireless_SelectParent(u32 index);
void func_0207bb48(int start);
void func_0207bbf8(void);
void func_0207bc14(void);
void LinkBanner_Create(int id);
void Link_SendUnflaggedPacket(void* data);
Task* LinkMenu_FindTask(void);
void Task_ResumeScript(Task* task);
void MI_CpuFill8(void* dest, int value, int size);

/* A on the player list, unless a choice was made. In link state 3, button 4
 * runs func_0207bc14 and the other buttons join the parent of that row; on
 * success it starts the banner of the button. The screen locks even when
 * joining fails. In state 4, once func_0207a170 is set and a child is
 * connected, it sends the children an ack holding the connected child bits,
 * closes the buttons and resumes the player list task. */
void LinkMenu_Confirm(void) {
  u8 ack[23];
  int id;

  if (data_0215fed0->locked == 1) {
    return;
  }
  switch (Wireless_GetState()) {
    /* needed to match: the empty cases make the jump table */
    case 0:
    case 1:
    case 2:
    case 5:
      break;
    case 3:
      /* needed to match: an int copy that is narrowed again for the call */
      if ((id = data_0215fed0->selected) == 4) {
        func_0207bc14();
        Sound_PlaySe(SE_CONFIRM);
        MenuButtons_RemoveSelectable();
        LinkBanner_Create(data_0215fed0->selected);
      } else if (Wireless_SelectParent((u8)id) == TRUE) {
        func_0207bbf8();
        Sound_PlaySe(SE_CONFIRM);
        MenuButtons_RemoveSelectable();
        LinkBanner_Create(data_0215fed0->selected);
      } else {
        Sound_PlaySe(SE_CANCEL);
      }
      data_0215fed0->locked = 1;
      return;
    case 4:
      if (func_0207a170() == 0) {
        return;
      }
      if (Wireless_GetChildAids() != 0) {
        func_0207bb48(0);
        MI_CpuFill8(ack, 0, sizeof(ack));
        ack[0] = LINK_UNFLAGGED_ACK;
        ack[1] = Wireless_GetChildAids();
        Link_SendUnflaggedPacket(ack);
        MenuButtons_CloseAll();
        Task_ResumeScript(LinkMenu_FindTask());
        Sound_PlaySe(SE_CONFIRM);
        data_0215fed0->locked = 1;
        return;
      }
      Sound_PlaySe(SE_CANCEL);
      break;
  }
}

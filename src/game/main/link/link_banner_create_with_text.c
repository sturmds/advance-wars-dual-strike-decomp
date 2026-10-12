#include "types.h"
#include "game/task.h"
#include "game/link_banner_task.h"

extern const TaskScriptCmd data_0215ff9c[];

/* Starts a banner that grows in and shows textId (LinkBanner_GrowBox). */
void LinkBanner_CreateWithText(void* parentOrList, int textId) {
  LinkBannerTask* task =
      (LinkBannerTask*)Task_Create(data_0215ff9c, parentOrList);
  task->textId = textId;
}

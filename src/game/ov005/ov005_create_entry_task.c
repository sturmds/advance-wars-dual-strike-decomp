#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"
#include "game/ov005_task.h"
#include "game/screen_contexts.h"

void Ov005Entry_WaitThenTurn(void);
void Ov005Screen_AllocLargeWork(void);
void Ov005Screen_EndSetFlag57(void);
void Ov005Screen_StartPaletteTask(void);
void Ov005Slide_Update(void);
void Ov005_SetUpMainScreenLayer2(void);
void Ov005_SetUpStaffCreditsScreen(void);
void Ov005_UpdateStaffCreditsScreen(void);
void Ov005_UpdateTimedScreen(void);
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_ov005_0235a81c(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

char gOv005MessageFile01c[] = {
#include "data/ov005/gOv005MessageFile01c.inc.c"
};
char gOv005MessageFile014[] = {
#include "data/ov005/gOv005MessageFile014.inc.c"
};
char gOv005MessageFile013[] = {
#include "data/ov005/gOv005MessageFile013.inc.c"
};
char gOv005MessageFile01d[] = {
#include "data/ov005/gOv005MessageFile01d.inc.c"
};
char gOv005MessageFile015[] = {
#include "data/ov005/gOv005MessageFile015.inc.c"
};
char gOv005MessageFile017[] = {
#include "data/ov005/gOv005MessageFile017.inc.c"
};
char gOv005MessageFile01b[] = {
#include "data/ov005/gOv005MessageFile01b.inc.c"
};
char gOv005MessageFile016[] = {
#include "data/ov005/gOv005MessageFile016.inc.c"
};
char gOv005MessageFile01e[] = {
#include "data/ov005/gOv005MessageFile01e.inc.c"
};
u8 data_ov005_023695c4[] = {
#include "data/ov005/data_ov005_023695c4.inc.c"
};
u8 data_ov005_023695d0[] = {
#include "data/ov005/data_ov005_023695d0.inc.c"
};
static TaskScriptCmd gOv005EntryTaskScript[] = {
#include "data/ov005/gOv005EntryTaskScript.inc.c"
};
TaskScriptCmd data_ov005_023695f4[] = {
#include "data/ov005/data_ov005_023695f4.inc.c"
};
TaskScriptCmd data_ov005_0236960c[] = {
#include "data/ov005/data_ov005_0236960c.inc.c"
};
TaskScriptCmd data_ov005_02369634[] = {
#include "data/ov005/data_ov005_02369634.inc.c"
};
GfxLayer data_ov005_0236966c[] = {
#include "data/ov005/data_ov005_0236966c.inc.c"
};
GfxLayer gOv005CreditsSubLayers[] = {
#include "data/ov005/gOv005CreditsSubLayers.inc.c"
};
u16 data_ov005_0236970c[] = {
#include "data/ov005/data_ov005_0236970c.inc.c"
};

Ov005Task* Ov005_CreateEntryTask(void* work, int entryIndex, int arg) {
  Ov005Task* task =
      (Ov005Task*)Task_Create(gOv005EntryTaskScript, TASK_LIST(3));

  task->work = work;
  task->entryIndex = entryIndex;
  task->unk_48 = arg;
  task->step = 0;
  task->unk_30 = 1;
  return task;
}

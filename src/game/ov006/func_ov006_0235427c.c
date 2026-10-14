#include "types.h"
#include "game/task.h"

void func_0201e954(void);
void func_ov006_023505c4(void);
void MenuDepth_Pop(void);
void func_ov006_0235432c(void);
void func_ov006_0235433c(void);
void func_ov006_023543c4(void);

/* layer 3 tiles */
char gMapeditFile006[4] = {
#include "data/ov006/gMapeditFile006.inc.c"
};
char gMapeditFile005[4] = {
#include "data/ov006/gMapeditFile005.inc.c"
};
char gMapeditFile004[4] = {
#include "data/ov006/gMapeditFile004.inc.c"
};
char gMapeditFile00a[4] = {
#include "data/ov006/gMapeditFile00a.inc.c"
};
char gMapeditFile009[4] = {
#include "data/ov006/gMapeditFile009.inc.c"
};
char gMapeditFile001[4] = {
#include "data/ov006/gMapeditFile001.inc.c"
};
/* layer 3 palette */
char gMapeditFile008[4] = {
#include "data/ov006/gMapeditFile008.inc.c"
};
/* layer 3 map */
char gMapeditFile007[4] = {
#include "data/ov006/gMapeditFile007.inc.c"
};
static TaskScriptCmd data_ov006_0236ab54[7] = {
#include "data/ov006/data_ov006_0236ab54.inc.c"
};
TaskScriptCmd data_ov006_0236ab8c[7] = {
#include "data/ov006/data_ov006_0236ab8c.inc.c"
};

void func_ov006_0235427c(void) {
  func_0201e954();
  Task_Create(data_ov006_0236ab54, TASK_LIST(3));
  func_ov006_023505c4();
}

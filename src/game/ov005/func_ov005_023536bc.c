#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"
#include "game/sound.h"
#include "game/screen_contexts.h"

void Ov005_SetUpMainScreen(void);
void Ov005_SetupBlendInScreensWithSong(void);
void Ov005_SetupMainScreenLayers2And3(void);
void Ov005_UpdateBlendEventScriptScreen(void);
void Ov005_UpdateScrollingEventScriptScreen(void);
void Ov005_UpdateSongEventScriptScreen(void);
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_ov005_023536d4(void);
void func_ov005_023551f8(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

char gRikiishiFile00d[] = {
#include "data/ov005/gRikiishiFile00d.inc.c"
};
char gRikiishiFile00e[] = {
#include "data/ov005/gRikiishiFile00e.inc.c"
};
char gRikiishiFile004[] = {
#include "data/ov005/gRikiishiFile004.inc.c"
};
char gRikiishiFile003[] = {
#include "data/ov005/gRikiishiFile003.inc.c"
};
char gRikiishiFile002[] = {
#include "data/ov005/gRikiishiFile002.inc.c"
};
char gRikiishiFile001[] = {
#include "data/ov005/gRikiishiFile001.inc.c"
};
char gRikiishiFile000[] = {
#include "data/ov005/gRikiishiFile000.inc.c"
};
char gRikiishiFile00c[] = {
#include "data/ov005/gRikiishiFile00c.inc.c"
};
char gRikiishiFile00b[] = {
#include "data/ov005/gRikiishiFile00b.inc.c"
};
char gRikiishiFile00a[] = {
#include "data/ov005/gRikiishiFile00a.inc.c"
};
char gRikiishiFile009[] = {
#include "data/ov005/gRikiishiFile009.inc.c"
};
char gRikiishiFile008[] = {
#include "data/ov005/gRikiishiFile008.inc.c"
};
char gRikiishiFile007[] = {
#include "data/ov005/gRikiishiFile007.inc.c"
};
char gRikiishiFile006[] = {
#include "data/ov005/gRikiishiFile006.inc.c"
};
char gRikiishiFile005[] = {
#include "data/ov005/gRikiishiFile005.inc.c"
};
char gRikiishiFile011[] = {
#include "data/ov005/gRikiishiFile011.inc.c"
};
char gRikiishiFile012[] = {
#include "data/ov005/gRikiishiFile012.inc.c"
};
char gRikiishiFile010[] = {
#include "data/ov005/gRikiishiFile010.inc.c"
};
char gRikiishiFile00f[] = {
#include "data/ov005/gRikiishiFile00f.inc.c"
};
static TaskScriptCmd data_ov005_023682e0[] = {
#include "data/ov005/data_ov005_023682e0.inc.c"
};
GfxLayer gOv005BlendInSubLayers[] = {
#include "data/ov005/gOv005BlendInSubLayers.inc.c"
};
GfxLayer data_ov005_02368378[] = {
#include "data/ov005/data_ov005_02368378.inc.c"
};

void func_ov005_023536bc(Task* parent) {
  Task_CreateChild(data_ov005_023682e0, parent);
}

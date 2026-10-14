#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"
#include "game/sound.h"
#include "game/screen_contexts.h"

void Ov005_SetupBlendInScreensByDifficulty(void);
void Ov005_SetupMainScreenAndStartSong(void);
void Ov005_UpdateBlendEventScriptScreenB(void);
void Ov005_UpdateEventScriptScreen(void);
void Ov005_UpdateEventScriptScreenEndSong(void);
void Ov005_UpdateEventScriptSongScreen(void);
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_ov005_02355280(void);
void func_ov005_0235560c(void);
void func_ov005_02355bb0(void);
void func_ov005_02355f80(void);
void func_ov005_02356768(void);
void func_ov005_02357804(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

/* layer 2 tiles */
char data_ov005_02368a14[] = {
#include "data/ov005/data_ov005_02368a14.inc.c"
};
/* layer 2 map */
char data_ov005_02368a18[] = {
#include "data/ov005/data_ov005_02368a18.inc.c"
};
char gRikiishiFile021[] = {
#include "data/ov005/gRikiishiFile021.inc.c"
};
char gRikiishiFile020[] = {
#include "data/ov005/gRikiishiFile020.inc.c"
};
char gRikiishiFile01f[] = {
#include "data/ov005/gRikiishiFile01f.inc.c"
};
char gRikiishiFile015[] = {
#include "data/ov005/gRikiishiFile015.inc.c"
};
char gRikiishiFile014[] = {
#include "data/ov005/gRikiishiFile014.inc.c"
};
char gRikiishiFile013[] = {
#include "data/ov005/gRikiishiFile013.inc.c"
};
char gRikiishiFile01b[] = {
#include "data/ov005/gRikiishiFile01b.inc.c"
};
char gRikiishiFile018[] = {
#include "data/ov005/gRikiishiFile018.inc.c"
};
char gRikiishiFile016[] = {
#include "data/ov005/gRikiishiFile016.inc.c"
};
char gRikiishiFile026[] = {
#include "data/ov005/gRikiishiFile026.inc.c"
};
char gRikiishiFile025[] = {
#include "data/ov005/gRikiishiFile025.inc.c"
};
char gRikiishiFile024[] = {
#include "data/ov005/gRikiishiFile024.inc.c"
};
char gRikiishiFile023[] = {
#include "data/ov005/gRikiishiFile023.inc.c"
};
char gRikiishiFile022[] = {
#include "data/ov005/gRikiishiFile022.inc.c"
};
/* layer 2 palette */
char data_ov005_02368a1c[] = {
#include "data/ov005/data_ov005_02368a1c.inc.c"
};
GfxLayer data_ov005_02368a58[] = {
#include "data/ov005/data_ov005_02368a58.inc.c"
};
GfxLayer gOv005BlendInSubLayers2[] = {
#include "data/ov005/gOv005BlendInSubLayers2.inc.c"
};
static TaskScriptCmd data_ov005_02368af8[] = {
#include "data/ov005/data_ov005_02368af8.inc.c"
};

void func_ov005_02355268(Task* parent) {
  Task_CreateChild(data_ov005_02368af8, parent);
}

#include "types.h"
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
char data_ov005_02368a14[4] = {
#include "data/ov005/data_ov005_02368a14.inc.c"
};
/* layer 2 map */
char data_ov005_02368a18[4] = {
#include "data/ov005/data_ov005_02368a18.inc.c"
};
char gRikiishiFile021[4] = {
#include "data/ov005/gRikiishiFile021.inc.c"
};
char gRikiishiFile020[4] = {
#include "data/ov005/gRikiishiFile020.inc.c"
};
char gRikiishiFile01f[4] = {
#include "data/ov005/gRikiishiFile01f.inc.c"
};
char gRikiishiFile015[4] = {
#include "data/ov005/gRikiishiFile015.inc.c"
};
char gRikiishiFile014[4] = {
#include "data/ov005/gRikiishiFile014.inc.c"
};
char gRikiishiFile013[4] = {
#include "data/ov005/gRikiishiFile013.inc.c"
};
char gRikiishiFile01b[4] = {
#include "data/ov005/gRikiishiFile01b.inc.c"
};
char gRikiishiFile018[4] = {
#include "data/ov005/gRikiishiFile018.inc.c"
};
char gRikiishiFile016[4] = {
#include "data/ov005/gRikiishiFile016.inc.c"
};
char gRikiishiFile026[4] = {
#include "data/ov005/gRikiishiFile026.inc.c"
};
char gRikiishiFile025[4] = {
#include "data/ov005/gRikiishiFile025.inc.c"
};
char gRikiishiFile024[4] = {
#include "data/ov005/gRikiishiFile024.inc.c"
};
char gRikiishiFile023[4] = {
#include "data/ov005/gRikiishiFile023.inc.c"
};
char gRikiishiFile022[4] = {
#include "data/ov005/gRikiishiFile022.inc.c"
};
/* layer 2 palette */
char data_ov005_02368a1c[4] = {
#include "data/ov005/data_ov005_02368a1c.inc.c"
};
/* declared `u8 data_ov005_02368a58[]` in func_ov005_0235560c.c: relocation at data_ov005_02368a58+0xc is not on a 4-byte field */
u32 data_ov005_02368a58[20] = {
#include "data/ov005/data_ov005_02368a58.inc.c"
};
/* declared `u8 gOv005BlendInSubLayers2[]` in ov005_setup_blend_in_screens_by_difficulty.c: relocation at gOv005BlendInSubLayers2+0xc is not on a 4-byte field */
u32 gOv005BlendInSubLayers2[20] = {
#include "data/ov005/gOv005BlendInSubLayers2.inc.c"
};
static TaskScriptCmd data_ov005_02368af8[13] = {
#include "data/ov005/data_ov005_02368af8.inc.c"
};

void func_ov005_02355268(Task* parent) {
  Task_CreateChild(data_ov005_02368af8, parent);
}

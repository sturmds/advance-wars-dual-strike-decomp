#include "types.h"
#include "sdk/gx.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bb90.h"
#include "sdk/os.h"

void G3i_PerspectiveW_(int fovSin, int fovCos, int aspect, int nearZ, int farZ,
                       int scale, BOOL load,
                       void* mtx); /* SDK perspective projection */
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void* File_Load(const char* path);
void Mem_Free(void* ptr);
void GX_BeginLoadTexPltt(void);
void GX_LoadTexPltt(const void* src, u32 offset,
                    u32 size); /* guess: copies to texture palette VRAM */
void GX_EndLoadTexPltt(void);

extern char data_ov005_0236988c[];
/* layer 2 map */
char data_ov005_0236989c[] = {
#include "data/ov005/data_ov005_0236989c.inc.c"
};
char gRikiishiFile060[] = {
#include "data/ov005/gRikiishiFile060.inc.c"
};
char gRikiishiFile05f[] = {
#include "data/ov005/gRikiishiFile05f.inc.c"
};
char data_ov005_023698c0[] = {
#include "data/ov005/data_ov005_023698c0.inc.c"
};
char data_ov005_023698bc[] = {
#include "data/ov005/data_ov005_023698bc.inc.c"
};
static char data_ov005_023698cc[] = {
#include "data/ov005/data_ov005_023698cc.inc.c"
};
char gRikiishiFile062[] = {
#include "data/ov005/gRikiishiFile062.inc.c"
};
char data_ov005_023698c4[] = {
#include "data/ov005/data_ov005_023698c4.inc.c"
};
char gRikiishiFile061[] = {
#include "data/ov005/gRikiishiFile061.inc.c"
};
/* layer 2 palette */
char data_ov005_023698a0[] = {
#include "data/ov005/data_ov005_023698a0.inc.c"
};
char gRikiishiFile063[] = {
#include "data/ov005/gRikiishiFile063.inc.c"
};
char gRikiishiFile05e[] = {
#include "data/ov005/gRikiishiFile05e.inc.c"
};
/* layer 2 tiles */
char data_ov005_02369898[] = {
#include "data/ov005/data_ov005_02369898.inc.c"
};
char data_ov005_023698c8[] = {
#include "data/ov005/data_ov005_023698c8.inc.c"
};
char data_ov005_023698d0[] = {
#include "data/ov005/data_ov005_023698d0.inc.c"
};
char data_ov005_023698e4[] = {
#include "data/ov005/data_ov005_023698e4.inc.c"
};
char data_ov005_023698d8[] = {
#include "data/ov005/data_ov005_023698d8.inc.c"
};
char data_ov005_023698f0[] = {
#include "data/ov005/data_ov005_023698f0.inc.c"
};
char data_ov005_02369908[] = {
#include "data/ov005/data_ov005_02369908.inc.c"
};
char data_ov005_023698fc[] = {
#include "data/ov005/data_ov005_023698fc.inc.c"
};
char data_ov005_02369914[] = {
#include "data/ov005/data_ov005_02369914.inc.c"
};
char data_ov005_02369920[] = {
#include "data/ov005/data_ov005_02369920.inc.c"
};
char data_ov005_0236992c[] = {
#include "data/ov005/data_ov005_0236992c.inc.c"
};
char data_ov005_02369950[] = {
#include "data/ov005/data_ov005_02369950.inc.c"
};
char data_ov005_02369944[] = {
#include "data/ov005/data_ov005_02369944.inc.c"
};
char data_ov005_0236995c[] = {
#include "data/ov005/data_ov005_0236995c.inc.c"
};
char data_ov005_02369968[] = {
#include "data/ov005/data_ov005_02369968.inc.c"
};
char data_ov005_02369974[] = {
#include "data/ov005/data_ov005_02369974.inc.c"
};
char data_ov005_02369938[] = {
#include "data/ov005/data_ov005_02369938.inc.c"
};
char data_ov005_02369980[] = {
#include "data/ov005/data_ov005_02369980.inc.c"
};
char data_ov005_0236998c[] = {
#include "data/ov005/data_ov005_0236998c.inc.c"
};
char data_ov005_02369998[] = {
#include "data/ov005/data_ov005_02369998.inc.c"
};
char data_ov005_023699a4[] = {
#include "data/ov005/data_ov005_023699a4.inc.c"
};
char data_ov005_023699b0[] = {
#include "data/ov005/data_ov005_023699b0.inc.c"
};
char data_ov005_023699bc[] = {
#include "data/ov005/data_ov005_023699bc.inc.c"
};
char data_ov005_02369a78[] = {
#include "data/ov005/data_ov005_02369a78.inc.c"
};
char data_ov005_023699c8[] = {
#include "data/ov005/data_ov005_023699c8.inc.c"
};
char data_ov005_023699d8[] = {
#include "data/ov005/data_ov005_023699d8.inc.c"
};
char data_ov005_023699e8[] = {
#include "data/ov005/data_ov005_023699e8.inc.c"
};
char data_ov005_023699f8[] = {
#include "data/ov005/data_ov005_023699f8.inc.c"
};
char data_ov005_02369a08[] = {
#include "data/ov005/data_ov005_02369a08.inc.c"
};
char data_ov005_02369a18[] = {
#include "data/ov005/data_ov005_02369a18.inc.c"
};
char data_ov005_02369a28[] = {
#include "data/ov005/data_ov005_02369a28.inc.c"
};
char data_ov005_02369a38[] = {
#include "data/ov005/data_ov005_02369a38.inc.c"
};
char data_ov005_02369a48[] = {
#include "data/ov005/data_ov005_02369a48.inc.c"
};
char data_ov005_02369a58[] = {
#include "data/ov005/data_ov005_02369a58.inc.c"
};
char data_ov005_02369a68[] = {
#include "data/ov005/data_ov005_02369a68.inc.c"
};
char data_ov005_02369ac8[] = {
#include "data/ov005/data_ov005_02369ac8.inc.c"
};
char data_ov005_02369ad8[] = {
#include "data/ov005/data_ov005_02369ad8.inc.c"
};
char data_ov005_02369ae8[] = {
#include "data/ov005/data_ov005_02369ae8.inc.c"
};
char data_ov005_02369af8[] = {
#include "data/ov005/data_ov005_02369af8.inc.c"
};
char data_ov005_02369b08[] = {
#include "data/ov005/data_ov005_02369b08.inc.c"
};
char data_ov005_02369a88[] = {
#include "data/ov005/data_ov005_02369a88.inc.c"
};
char data_ov005_02369a98[] = {
#include "data/ov005/data_ov005_02369a98.inc.c"
};
char data_ov005_02369aa8[] = {
#include "data/ov005/data_ov005_02369aa8.inc.c"
};
char data_ov005_02369ab8[] = {
#include "data/ov005/data_ov005_02369ab8.inc.c"
};
char data_ov005_02369b48[] = {
#include "data/ov005/data_ov005_02369b48.inc.c"
};
char data_ov005_02369b58[] = {
#include "data/ov005/data_ov005_02369b58.inc.c"
};
char data_ov005_02369b78[] = {
#include "data/ov005/data_ov005_02369b78.inc.c"
};
char data_ov005_02369b88[] = {
#include "data/ov005/data_ov005_02369b88.inc.c"
};
char data_ov005_02369b18[] = {
#include "data/ov005/data_ov005_02369b18.inc.c"
};
char data_ov005_02369b28[] = {
#include "data/ov005/data_ov005_02369b28.inc.c"
};
char data_ov005_02369b68[] = {
#include "data/ov005/data_ov005_02369b68.inc.c"
};
char data_ov005_02369b98[] = {
#include "data/ov005/data_ov005_02369b98.inc.c"
};
char data_ov005_02369b38[] = {
#include "data/ov005/data_ov005_02369b38.inc.c"
};
char data_ov005_02369ba8[] = {
#include "data/ov005/data_ov005_02369ba8.inc.c"
};
char data_ov005_02369bb8[] = {
#include "data/ov005/data_ov005_02369bb8.inc.c"
};

/* On the main screen: sets up the 3D engine with a perspective projection
 * (the same steps as Ov004_SetupPerspective3d), unpacks a file into
 * work->buffer_0010 and loads 32 bytes of texture palette from another.
 * Then clears both entry lists. */
void Ov005Screen_Init3dAndEntries(Unk_ov005_0236bb90* work) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  void* palette;
  int i;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  *REG_DISP3DCNT &=
      0xcfef; /* anti-aliasing off, acknowledge the overflow flags */
  *REG_SWAP_BUFFERS = 3;
  G3X_SetClearColor(0x2bea, 0, 0x7fff, 63, FALSE);
  *REG_VIEWPORT = 0xbfff0000; /* (0, 0) to (255, 191) */
  G3i_PerspectiveW_(0x800, 0xddb, 0x1555, 1, 0x2000, 0x1000, TRUE, NULL);
  *REG_MTX_STORE = 0;
  FS_ChangeDir(data_ov005_0236988c);
  File_UnpackTo(work->buffer_0010, data_ov005_023698cc);
  palette = File_Load(data_ov005_023698bc);
  DC_FlushRange(palette, 32);
  GX_BeginLoadTexPltt();
  GX_LoadTexPltt(palette, 0, 32);
  GX_EndLoadTexPltt();
  Mem_Free(palette);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  for (i = 0; i < OV005_SLIDE_ENTRIES; i++) {
    work->entriesA[i].id = 0;
    work->entriesA[i].textLength = 0;
    work->entriesA[i].text = NULL;
    work->entriesA[i].unk_08 = 0;
    work->entriesA[i].kind = 0;
    work->entriesA[i].angle = 0;
    work->entriesA[i].offsetX = 0;
    work->entriesA[i].pos = 0;
    work->entriesA[i].unk_12 = 0;
    work->entriesA[i].slideTask = NULL;
    work->entriesB[i].id = 0;
    work->entriesB[i].textLength = 0;
    work->entriesB[i].text = NULL;
    work->entriesB[i].unk_08 = 0;
    work->entriesB[i].kind = 0;
    work->entriesB[i].angle = 0;
    work->entriesB[i].offsetX = 0;
    work->entriesB[i].pos = 0;
    work->entriesB[i].unk_12 = 0;
    work->entriesB[i].slideTask = NULL;
  }
}

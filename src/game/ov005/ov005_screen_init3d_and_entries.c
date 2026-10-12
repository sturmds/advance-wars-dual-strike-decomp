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
extern char data_ov005_023698bc[];
extern char data_ov005_023698cc[];

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

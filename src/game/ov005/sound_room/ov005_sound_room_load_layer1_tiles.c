#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bbac.h"
#include "game/screen_other_map.h"
#include "sdk/fs.h"

void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);

extern char data_ov005_0236b324[];
extern char data_ov005_0236b334[];
extern char data_ov005_0236b338[];

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

/* Unpacks a tile file into the second screen's layer 1 character data, 0x6000
 * bytes after its base, and loads a 32-byte palette file at offset 64. */
void Ov005SoundRoom_LoadLayer1Tiles(void) {
  Unk_ov005_0236bbac* state = data_ov005_0236bbac;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  FS_ChangeDir(data_ov005_0236b324);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14) +
          0x6000,
      data_ov005_0236b334);
  Palette_LoadFromFile(data_ov005_0236b338, 0, 64, 32);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  state->unk_1114 = 0;
}

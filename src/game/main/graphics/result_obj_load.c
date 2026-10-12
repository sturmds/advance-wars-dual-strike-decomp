#include "types.h"
#include "game/Unk_02088d04.h"

void* File_Load(const char* path);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, u32 offset, u32 size);

extern char data_0215a4b4[]; /* "/ohashi/res_result_obj" */
extern Unk_027c0280* data_027c0280;

/* Loads the result screen's sprite file, unpacks its tiles to OBJ VRAM at
 * firstTile and its palette to OBJ palette slot `palette`. Returns the file,
 * which the caller frees. */
void* ResultObj_Load(int firstTile, int palette) {
  u8* file = File_Load(data_0215a4b4);

  Data_Unpack(file + 0x1024, data_027c0280->objVram + firstTile * 32);
  /* OBJ palettes follow the 16 BG palettes; the u32 cast keeps the shift logical. */
  Palette_Load(file + 0x14cc, ((u32)(palette + 16) << 21) >> 16, 32);
  return file;
}

#include "types.h"
#include "game/Unk_02088d04.h"

u8* File_Load(const char* path);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, u32 offset, u32 size);
void Mem_Free(void* ptr);

extern const char data_02160608[];

/* Loads the weather effect sprite tiles and palette into the OBJ areas of the screen. */
void WeatherFx_LoadGraphics(void) {
  u8* file = File_Load(data_02160608);

  Data_Unpack(file, data_027c0280->objVram + 0x1280);
  Palette_Load(file + 0x568, 0x300, 32);
  Mem_Free(file);
}

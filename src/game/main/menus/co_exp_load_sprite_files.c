#include "types.h"
#include "game/menu_state.h"

void* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Free(void* block);

extern const char data_0215a400[];
extern const char data_0215a41c[];
extern MenuState gMenuState;

/* Copies two sprite files into the CO experience screen's work area: the
 * first (0x160 bytes, localized path) at gMenuState + 0x1364, the second
 * (0xe4 bytes) right after it at + 0x14c4. */
void CoExp_LoadSpriteFiles(void) {
  void* file;

  file = File_Load(File_LocalizePath(data_0215a400));
  Mem_Copy32(file, (u8*)&gMenuState + 0x1364, 0x160);
  Mem_Free(file);
  file = File_Load(data_0215a41c);
  Mem_Copy32(file, (u8*)&gMenuState + 0x14c4, 0xe4);
  Mem_Free(file);
}

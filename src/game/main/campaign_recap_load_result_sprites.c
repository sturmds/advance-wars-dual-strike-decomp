#include "types.h"
#include "game/campaign_recap.h"
#include "game/Unk_02088d04.h"
#include "sdk/fs.h"

extern const char data_0216a928[]; /* "/rikiishi" */
extern const char data_0216a940[]; /* "/ohashi/res_result_obj" */
extern const char data_0216a958[]; /* "/ohashi/res_result_point" */
extern const char data_0216a978[]; /* "05c" */
extern const char data_0216a97c[]; /* "05d" */

u8* File_Load(const char* path);
void Data_Unpack(const void* src, void* dst);
void Mem_Free(void* block);
void Palette_Load(const void* src, u32 offset, u32 size);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);

/* Loads the recap result sprites (tiles into OBJ VRAM, palettes) from
 * res_result_obj, res_result_point and rikiishi/05c-05d. work is unused. */
void CampaignRecap_LoadResultSprites(CampaignRecap* work) {
  u8* file;

  file = File_Load(data_0216a940);
  Data_Unpack(file, data_027c0280->objVram + 0x3000);
  Data_Unpack(file + 0x56c, data_027c0280->objVram + 0x3800);
  Data_Unpack(file + 0x9e4, data_027c0280->objVram + 0x4000);
  Data_Unpack(file + 0xd54, data_027c0280->objVram + 0x4800);
  Data_Unpack(file + 0xfb4, data_027c0280->objVram + 0x5000);
  Palette_Load(file + 0x152c, 0x240, 0x80);
  Mem_Free(file);

  file = File_Load(data_0216a958);
  Data_Unpack(file + 0x39c, data_027c0280->objVram + 0x5100);
  Data_Unpack(file + 0xfc, data_027c0280->objVram + 0x5880);
  Palette_Load(file + 0x14cc, 0x2e0, 0x40);
  Mem_Free(file);

  FS_ChangeDir(data_0216a928);
  File_UnpackTo(data_027c0280->objVram + 0x7000, data_0216a978);
  Palette_LoadFromFile(data_0216a97c, 0, 0x320, 0x20);
}

#include "types.h"

void File_ReadAt(void* dest, const char* path, u32 offset, u32 size);

extern char data_02159928[];

/* Reads one 128-byte skill icon (four 8x8 tiles) from /ohashi/res_skill
 * (data_02159928) into dest. Skill 0 takes the icon at offset 32;
 * skills 32 to 127 take theirs from offset 160 on. Other ids read nothing. */
void SkillIcon_Load(int skill, void* dest) {
  if (skill == 0) {
    File_ReadAt(dest, data_02159928, 32, 128);
  } else if (skill >= 32) {
    if (skill < 128) {
      File_ReadAt(dest, data_02159928, (skill - 32) * 128 + 160, 128);
    }
  }
}

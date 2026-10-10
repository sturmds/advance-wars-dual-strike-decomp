#include "types.h"
#include "game/screen_other_map.h"

const char *File_LocalizePath(const char *path);
void File_UnpackTo(void *dest, const char *fileName);
extern char data_021598f4[];

void func_0202b6f8(void *dest)
{
    Unk_027c0280 *savedGfx = data_027c0280;
    Unk_02080440 *savedState;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    File_UnpackTo(dest, File_LocalizePath(data_021598f4));
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
}

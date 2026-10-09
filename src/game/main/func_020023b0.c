#include "types.h"

void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
int func_02002bac(int coId, int army);
void func_02002c88(int coId, int paletteSlot, int army);
extern u32 data_0229eaec;
extern char data_02151e44[];
extern const char *const data_02152b48[][7][3];

void func_020023b0(int coMood, void *dest, int paletteSlot, int army)
{
    int variant;

    if (data_0229eaec == 0 || (coMood & 0x80)) {
        army = 0;
    }
    FS_ChangeDir(data_02151e44);
    variant = func_02002bac(coMood, army);
    File_UnpackTo(dest, data_02152b48[coMood & 0x7f][variant][(coMood & 0x300) >> 8]);
    func_02002c88(coMood, paletteSlot, army);
}

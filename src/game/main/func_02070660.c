#include "types.h"

typedef struct Unk_02070660_0 {
  u8 pad_00[4];
  u8 unk_04;
} Unk_02070660_0;

extern Unk_02070660_0 gMenuState;
void Sound_PlaySe(int id);
void TabList_RebuildButtons(void);
void TabList_UpdateHelpText(void);

void func_02070660(void) {
  Sound_PlaySe(0x10002);
  gMenuState.unk_04 = 3;
  TabList_RebuildButtons();
  TabList_UpdateHelpText();
}

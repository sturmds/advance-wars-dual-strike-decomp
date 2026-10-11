#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern s16 data_0213936c[2];

void func_020e16e0(int x);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts the side panel on the right, at its resting x (data_0213936c[1]). */
void SidePanel_SnapRight(void)
{
    STATE->sidePanelRight = 1;
    STATE->sidePanelX = data_0213936c[1];
    func_020e16e0(STATE->sidePanelX);
}

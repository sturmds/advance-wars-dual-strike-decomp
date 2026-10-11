#include "types.h"
#include "game/screen_other_map.h"

void GameFlagsMid_Backup(void);
void func_02064774(void);
void LinkMenu_Start(void);

/* Same as LinkMenu_StartTwoMaps for a one-map battle, with unk_8b 0. */
void LinkMenu_StartOneMap(void)
{
    GameFlagsMid_Backup();
    func_02064774();
    data_027c027c->unk_48[data_027c0284]->unk_8b = 0;
    data_027c027c->unk_48[data_027c0284]->twoMaps = 0;
    LinkMenu_Start();
}

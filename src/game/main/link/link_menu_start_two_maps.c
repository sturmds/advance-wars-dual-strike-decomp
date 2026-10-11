#include "types.h"
#include "game/screen_other_map.h"

void GameFlagsMid_Backup(void);
void func_02064774(void);
void LinkMenu_Start(void);

/* Copies the game flags (GameFlagsMid_Backup), then opens the link menu for a
 * two-map battle with unk_8b 1, which picks the login screen title. */
void LinkMenu_StartTwoMaps(void)
{
    GameFlagsMid_Backup();
    func_02064774();
    data_027c027c->unk_48[data_027c0284]->unk_8b = 1;
    data_027c027c->unk_48[data_027c0284]->twoMaps = 1;
    LinkMenu_Start();
}

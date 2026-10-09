#include "types.h"
#include "game/Unk_02080440.h"

void BuildUnlockedCoList(void);
void LoadMapCoList(int mapId);
BOOL Campaign_IsHardMode(void);
void func_02045860(void);
void func_02045874(int coId);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_022a7a68[];

/* Fills the CO list data_022a7a68: the current map's own list when
 * state->gameMode and Campaign_IsHardMode() are both 0, otherwise every unlocked CO.
 * Then clears the CO bit mask (func_02045860) and sets the bit of each listed
 * CO (func_02045874). */
void SetupCoSelection(void)
{
    int i;

    if (data_027c027c->unk_48[data_027c0284]->gameMode == 0) {
        if (Campaign_IsHardMode() == 0) {
            LoadMapCoList(data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId);
        } else {
            BuildUnlockedCoList();
        }
    } else {
        BuildUnlockedCoList();
    }
    func_02045860();
    for (i = 0; data_022a7a68[i] != 0; i++) {
        func_02045874(data_022a7a68[i]);
    }
}

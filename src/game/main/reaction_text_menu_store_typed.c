#include "types.h"
#include "game/reaction_text_menu_task.h"

ReactionTextMenuTask *Task_FindByScript(const void *script);
int func_020b983c(void);
u8 *func_021016e4(void);
char *func_021246d4(char *dst, const char *src);

extern u8 data_0215fc0c[];
extern char data_0229da8c[];

/* Copies the text typed in the name entry into the chosen reaction text. */
void ReactionTextMenu_StoreTyped(void)
{
    ReactionTextMenuTask *menu = Task_FindByScript(data_0215fc0c);

    if (func_020b983c() != 1) {
        return;
    }
    func_021246d4(&data_0229da8c[menu->selected * REACTION_TEXT_SIZE], (char *)func_021016e4());
    menu->done = 1;
}

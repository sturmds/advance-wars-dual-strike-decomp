#include "types.h"
#include "game/reaction_text_menu_task.h"

ReactionTextMenuTask *Task_FindByScript(const void *script);
void NameEntry_ClearBuffer(void);
u8 *func_021016e4(void);
char *func_021246d4(char *dst, const char *src);
void NameEntry_StartTask(int arg, int size);

extern u8 data_0215fc0c[];
extern char data_0229da8c[];

/* Loads the chosen reaction text into the name entry buffer. */
void ReactionTextMenu_LoadIntoEntry(int arg)
{
    ReactionTextMenuTask *menu = Task_FindByScript(data_0215fc0c);

    NameEntry_ClearBuffer();
    func_021246d4((char *)func_021016e4(), &data_0229da8c[menu->selected * REACTION_TEXT_SIZE]);
    NameEntry_StartTask(arg, 16);
}

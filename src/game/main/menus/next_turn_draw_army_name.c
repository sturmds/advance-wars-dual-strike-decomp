#include "types.h"
#include "game/icon.h"

extern u32 gArmyNameTextIds[];                 /* text id of each army's name */

const char *Text_Get(u32 textId);
int Text_GetCenteredX(const char *text);
void Text_DrawCentered(int y, const char *text);
int strlen(const char *s);

/* First army emblem icon; the army colour is added to it. */
#define ICON_ARMY_EMBLEM 79

/* Draws the army's name centered at row y, with the army emblem on both
 * sides of it. */
void NextTurn_DrawArmyName(int y, int army)
{
    const char *name = Text_Get(gArmyNameTextIds[army]);
    int x = Text_GetCenteredX(name);

    Text_DrawCentered(y, name);
    Icon_Draw(ICON_ARMY_EMBLEM + army, x - 16, y - 4, 0, 0);
    Icon_Draw(ICON_ARMY_EMBLEM + army, x + strlen(name) * 8, y - 4, 0, 0);
}

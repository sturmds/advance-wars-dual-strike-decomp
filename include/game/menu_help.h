#ifndef GAME_MENU_HELP_H
#define GAME_MENU_HELP_H

#include "types.h"

/* Same arguments as the menu handler, which passes its own on; the help
 * text functions these tables point to take none, so the tables cast them. */
typedef void (*MenuHelpFn)(int a0, int a1, u32 flags);

#define MENU_HELP_MUSIC_STATES 2 /* off, on */
#define MENU_HELP_AI_BEHAVIOURS \
  4 /* AI_BEHAVIOUR_STRIKE to AI_BEHAVIOUR_DEFENSE */
#define MENU_HELP_VISUAL_MODES 5 /* VISUAL_NONE to VISUAL_D */

/* One help text function per setting value. Structs, so that the menu
 * handlers copy a table to the stack as a whole, as the code does. */
typedef struct MenuHelpPair {
  MenuHelpFn fn[MENU_HELP_MUSIC_STATES];
} MenuHelpPair;

typedef struct MenuHelpFour {
  MenuHelpFn fn[MENU_HELP_AI_BEHAVIOURS];
} MenuHelpFour;

typedef struct MenuHelpFive {
  MenuHelpFn fn[MENU_HELP_VISUAL_MODES];
} MenuHelpFive;

extern MenuHelpPair gMusicMenuHelpFns;       /* MenuToggle_Music */
extern MenuHelpFour gAiBehaviourMenuHelpFns; /* the AI behaviour setting */
extern MenuHelpFive gAnimationMenuHelpFns;   /* the Visual setting */

#endif

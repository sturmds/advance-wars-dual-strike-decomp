#ifndef GAME_VISUAL_MODE_H
#define GAME_VISUAL_MODE_H

/* Values of the "Visual" setting (PlayRecords.visualMode,
 * Unk_02080440_48.visualMode, MenuState.setup.visualMode), in the order of
 * the setting's help texts. */
enum VisualMode {
  VISUAL_NONE = 0, /* "No animation." */
  VISUAL_A = 1,    /* "Battle and capture animation are on."; the default */
  VISUAL_B = 2,    /* "Unit speed: Fast. Animation: Full." */
  VISUAL_C = 3,    /* "Fast. Animation: Battle." */
  VISUAL_D = 4     /* "Fast. Anim: No CPU battle." */
};

#endif

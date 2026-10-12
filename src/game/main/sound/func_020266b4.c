#include "types.h"

/* Like Sound_FadeMusicVolume, for the sound handle at gSeHandle instead of the
 * song's: sets the volume to from (-1: keep it), then fades to to over
 * frames frames. */
void Sound_FadeSeVolume(int from, int to, int frames);

/* Fades the gSeHandle sound out over 60 frames. Reached through a
 * pointer in ov002 (battle animation). */
void func_020266b4(void) { Sound_FadeSeVolume(-1, 0, 60); }

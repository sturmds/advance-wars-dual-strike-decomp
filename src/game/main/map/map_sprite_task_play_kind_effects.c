#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_sprite_task.h"
#include "game/screen.h"

u16 func_02021b20(SpriteAnim* sprite);
void Sound_PlaySe(u32 soundId);
/* Both start a child task of parent with the given arguments. */

/* The usual case body: the sound, then both effect tasks with their small
 * arguments; in game mode 4 only the first one. Ends the function. */
#define PLAY_SMALL_EFFECTS(sound)                            \
  Sound_PlaySe(sound);                                       \
  ScreenShake_Start(1, 20, task->parent);                    \
  if (data_027c027c->unk_48[data_027c0284]->gameMode == 4) { \
    return;                                                  \
  }                                                          \
  ScreenFlash_Start(4, 0, 2, task->parent);                  \
  return

/* Once the sprite reports an event (func_02021b20 returns it and clears
 * it), plays the sound for the task's kind and starts the effect tasks.
 * Kinds 25 and 26 get the large arguments, kind 30 no sound; kinds 25,
 * 26, 28, 29 and 30 skip the game mode 4 check. Kind 27 and kinds above
 * 35 use the default, the same as kind 0. The original has a copy of the
 * code in every case; the macro writes those copies. */
void MapSpriteTask_PlayKindEffects(MapSpriteTask* task) {
  if (func_02021b20(task->sprite) == 0) {
    return;
  }
  switch (task->kind) {
    case 30:
      ScreenFlash_Start(4, 0, 2, task->parent);
      return;
    case 29:
      Sound_PlaySe(0xb2);
      ScreenShake_Start(1, 20, task->parent);
      ScreenFlash_Start(4, 0, 2, task->parent);
      return;
    case 26:
      Sound_PlaySe(0x92);
      ScreenShake_Start(2, 90, task->parent);
      ScreenFlash_Start(90, 15, 30, task->parent);
      return;
    case 25:
      Sound_PlaySe(0x93);
      ScreenShake_Start(2, 90, task->parent);
      ScreenFlash_Start(90, 15, 30, task->parent);
      return;
    case 28:
      Sound_PlaySe(0xae);
      ScreenShake_Start(1, 20, task->parent);
      ScreenFlash_Start(4, 0, 2, task->parent);
      return;
    case 0:
      PLAY_SMALL_EFFECTS(0xa3);
    case 1:
      PLAY_SMALL_EFFECTS(0x79);
    case 2:
      PLAY_SMALL_EFFECTS(0x7a);
    case 3:
      PLAY_SMALL_EFFECTS(0x7b);
    case 4:
      PLAY_SMALL_EFFECTS(0x7c);
    case 5:
      PLAY_SMALL_EFFECTS(0x7d);
    case 6:
      PLAY_SMALL_EFFECTS(0x7e);
    case 7:
      PLAY_SMALL_EFFECTS(0x7f);
    case 9:
      PLAY_SMALL_EFFECTS(0x81);
    case 10:
      PLAY_SMALL_EFFECTS(0x82);
    case 8:
      PLAY_SMALL_EFFECTS(0x80);
    case 11:
      PLAY_SMALL_EFFECTS(0x83);
    case 12:
      PLAY_SMALL_EFFECTS(0x86);
    case 13:
      PLAY_SMALL_EFFECTS(0x87);
    case 14:
      PLAY_SMALL_EFFECTS(0x88);
    case 15:
      PLAY_SMALL_EFFECTS(0x89);
    case 16:
      PLAY_SMALL_EFFECTS(0x8a);
    case 17:
      PLAY_SMALL_EFFECTS(0x8b);
    case 18:
      PLAY_SMALL_EFFECTS(0x8c);
    case 19:
      PLAY_SMALL_EFFECTS(0x8d);
    case 20:
      PLAY_SMALL_EFFECTS(0x8e);
    case 21:
      PLAY_SMALL_EFFECTS(0x8f);
    case 22:
      PLAY_SMALL_EFFECTS(0x90);
    case 23:
      PLAY_SMALL_EFFECTS(0x91);
    case 24:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
      PLAY_SMALL_EFFECTS(0x85);
    default:
      PLAY_SMALL_EFFECTS(0xa3);
  }
}

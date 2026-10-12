#include "types.h"
#include "game/task.h"
#include "game/Unk_02088d04.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
struct SpriteAnimTask* SpriteAnimTask_Start(const void* file, int x, int y,
                                            u32 tileBase, int animation,
                                            int layer);
void Task_ResumeScript(Task* task);

extern Unk_027c0280* data_027c0280;
extern u8
    gMenuState[]; /* holds the sprite file the banner's load step copies in */

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define SLIDE_FRAMES 48

/* OAM attribute 1 bit 13, passed in with the x coordinate. */
#define SPRITE_FLIP_V 0x2000

/* OBJ palette 3, first tile 0x194: where the load step put the sprite. */
#define EDGE_SPRITE_TILES 0x3194

/* Step of the Super CO Power cut-in: slides the banner on BG0 from scroll
 * 204 to -52 over 48 frames, then starts five sprites along its top edge
 * (flipped) and five along its bottom edge, 64 pixels apart. */
void SuperBreak_SlideInBanner(Task* task) {
  int i;
  int x;
  u8 bg;

  FRAME(task)++;
  bg = data_027c0280->layer0Bg;
  data_027c0280->bgScrollY[bg] =
      Math_Ease(1, 204, -52, FRAME(task), SLIDE_FRAMES);
  if (FRAME(task) != SLIDE_FRAMES) {
    return;
  }
  for (i = 0; i < 5; i++) {
    x = i * 64 + 24;
    SpriteAnimTask_Start(gMenuState, (x & 0x1ff) | SPRITE_FLIP_V, 36,
                         EDGE_SPRITE_TILES, 0, 0);
    SpriteAnimTask_Start(gMenuState, x & 0x1ff, 140, EDGE_SPRITE_TILES, 0, 0);
  }
  FRAME(task) = 0;
  Task_ResumeScript(task);
}

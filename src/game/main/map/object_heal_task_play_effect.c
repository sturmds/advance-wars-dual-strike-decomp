#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/map_object.h"
#include "game/object_heal_task.h"
#include "game/sprite_anim.h"

extern u8 data_0213e078[]; /* Black Crystal heal animation */
extern u8 data_0213e2a0[]; /* Black Obelisk heal animation */

SpriteAnim3dTask* SpriteAnim3dTask_StartAlt(const void* file,
                                            const VecFx32* pos, u32 tileBase,
                                            u32 unk28, int animation,
                                            int layer);
void Sound_PlaySe(u32 soundId);

/* Sprite tile base passed for the 2D animation. */
#define HEAL_EFFECT_TILE_BASE 0x3194

/* Step of the object heal script: starts the heal animation on the object
 * and plays its sound. Drawn in 3D on the screen that shows the 3D engine,
 * as a sprite on the other. */
void ObjectHealTask_PlayEffect(ObjectHealTask* task) {
  VecFx32 crystalPos; /* needed to match: one position per case */
  VecFx32 obeliskPos;

  switch (task->objectKind) {
    case MAP_OBJECT_BLACK_CRYSTAL:
      if (data_027c027c == data_027c0288.unk_00) {
        crystalPos.x = (task->tileX * 16 + 8) * 0x2000 / 256;
        crystalPos.y = (task->tileY * 16 + 16) * 0x6000 / 768;
        crystalPos.z = 8;
        SpriteAnim3dTask_StartAlt(data_0213e078, &crystalPos,
                                  (u32)data_022a7c5c->unk_68,
                                  (u32)data_022a7c5c->unk_6c, 0, 1);
      } else {
        SpriteAnimTask_Start(
            data_0213e078, task->tileX * 16 + 8 - MAP_STATE->unk_1c4->scrollX,
            task->tileY * 16 + 16 - MAP_STATE->unk_1c4->scrollY,
            HEAL_EFFECT_TILE_BASE, 0, 1);
      }
      Sound_PlaySe(175);
      break;
    case MAP_OBJECT_BLACK_OBELISK:
      if (data_027c027c == data_027c0288.unk_00) {
        obeliskPos.x = (task->tileX * 16 + 8) * 0x2000 / 256;
        obeliskPos.y = (task->tileY * 16 + 8) * 0x6000 / 768;
        obeliskPos.z = 8;
        SpriteAnim3dTask_StartAlt(data_0213e2a0, &obeliskPos,
                                  (u32)data_022a7c5c->unk_68,
                                  (u32)data_022a7c5c->unk_6c, 0, 1);
      } else {
        SpriteAnimTask_Start(
            data_0213e2a0, task->tileX * 16 + 24 - MAP_STATE->unk_1c4->scrollX,
            task->tileY * 16 + 24 - MAP_STATE->unk_1c4->scrollY,
            HEAL_EFFECT_TILE_BASE, 0, 1);
      }
      Sound_PlaySe(176);
      break;
  }
}

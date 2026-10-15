#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"

/* Sets the rotation and scaling matrix (8.8 fixed point) that sprites with
 * that affine group number use. */
void Oam_SetAffineParams(int group, s16 pa, s16 pb, s16 pc, s16 pd) {
  ((OamAffineGroup*)data_027c0280->oamBuffer)[group].pa = pa;
  ((OamAffineGroup*)data_027c0280->oamBuffer)[group].pb = pb;
  ((OamAffineGroup*)data_027c0280->oamBuffer)[group].pc = pc;
  ((OamAffineGroup*)data_027c0280->oamBuffer)[group].pd = pd;
}

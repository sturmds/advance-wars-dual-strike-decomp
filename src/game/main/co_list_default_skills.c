#include "types.h"

void CoSet_ListEquippedSkills(u8* out, int co, int set);

/* Skill ids equipped in set 0 of a CO, ended by 0xff. */
void Co_ListDefaultSkills(u8* out, int co) {
  CoSet_ListEquippedSkills(out, co, 0);
}

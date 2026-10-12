#include "types.h"

/* BIOS GetPitchTable: an unsigned frequency factor for one 1/768 octave step, minus 0x10000. */
extern int func_arm7_037fed94(int index);

#define PITCH_STEPS_IN_OCTAVE 768
#define VOICE_RELOAD_FLOOR 0x10
#define VOICE_RELOAD_CEILING 0xFFFF

/* Scales a channel timer for a pitch shift given in 1/64 semitones (768 per octave). */
u16 Sound_PitchShiftTimer_Arm7(int timer, int pitch) {
  int octave = 0;
  int step = -pitch;
  int shift;
  u64 result;

  while (step < 0) {
    octave--;
    step += PITCH_STEPS_IN_OCTAVE;
  }
  while (step >= PITCH_STEPS_IN_OCTAVE) {
    octave++;
    step -= PITCH_STEPS_IN_OCTAVE;
  }
  result = (u32)func_arm7_037fed94(step);
  result += 0x10000;
  result *= timer;
  shift = octave - 16;
  if (shift <= 0) {
    shift = -shift;
    result >>= shift;
  } else if (shift < 32) {
    u64 mask = ~(u64)0 << (32 - shift);
    if ((result & mask) != 0) {
      return VOICE_RELOAD_CEILING;
    }
    result <<= shift;
  } else {
    return VOICE_RELOAD_CEILING;
  }
  if (result < VOICE_RELOAD_FLOOR) {
    result = VOICE_RELOAD_FLOOR;
  } else if (result > VOICE_RELOAD_CEILING) {
    result = VOICE_RELOAD_CEILING;
  }
  return result;
}

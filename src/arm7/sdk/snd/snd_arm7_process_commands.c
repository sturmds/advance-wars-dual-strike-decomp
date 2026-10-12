#include "sdk/os_arm7.h"
#include "sdk/snd.h"
#include "sdk/snd_hw.h"

void func_arm7_037fe534(int volume); /* writes the master volume register */
int func_arm7_03801e94(
    int varNo,
    int value); /* sets a global sequence variable in the shared work */

/*
 * Runs every command list the ARM9 has queued. Each list is a chain of
 * SndCommand; after a list the shared work's finishCommandTag goes up by one
 * so the ARM9 knows it ran. Several commands pack their arguments into bit
 * fields; the ARM9 writer side packs them the same way.
 */
void SndArm7_ProcessCommands(void) {
  OSMessage message;
  const SndCommand* next;
  SndCommand command;

  while (MsgQueue_PopOldest_Arm7(&data_arm7_03809210, &message,
                                 OS_MESSAGE_NOBLOCK)) {
    next = (const SndCommand*)message;
    while (next != NULL) {
      command = *next;
      switch (
          command
              .id) { /* needed to match: cases in the order of the original's code blocks */
        case SND_COMMAND_START_SEQ:
          SndArm7_PrepareSeq(command.args[0], (u8*)command.args[1],
                             command.args[2], (SoundBank*)command.args[3]);
          break;
        case SND_COMMAND_STOP_SEQ:
          SndArm7_StopSeq(command.args[0]);
          break;
        case SND_COMMAND_PAUSE_SEQ:
          SndArm7_PauseSeq(command.args[0], command.args[1]);
          break;
        case SND_COMMAND_SKIP_SEQ:
          SndArm7_SkipSeq(command.args[0], command.args[1]);
          break;
        case SND_COMMAND_PLAYER_PARAM:
          SndArm7_SetPlayerParam(command.args[0], command.args[1],
                                 command.args[2], command.args[3]);
          break;
        case SND_COMMAND_TRACK_PARAM:
          /* player number in bits 0-23, value size in bits 24-31; the u8 cast is needed to match */
          SndArm7_SetTrackParam(command.args[0] & 0xFFFFFF, command.args[1],
                                command.args[2], command.args[3],
                                (u8)(command.args[0] >> 24));
          break;
        case SND_COMMAND_MUTE_TRACK:
          SndArm7_SetTrackMute(command.args[0], command.args[1],
                               command.args[2]);
          break;
        case SOUND_REQ_LIMIT_TRACK_CHANNELS:
          SndArm7_SetTrackChannelMask(command.args[0], command.args[1],
                                      command.args[2]);
          break;
        case SOUND_REQ_SET_PLAYER_VAR:
          SndArm7_SetPlayerVariable(command.args[0], command.args[1],
                                    (s16)command.args[2]);
          break;
        case SOUND_REQ_SET_SHARED_VAR:
          func_arm7_03801e94(command.args[0], (s16)command.args[1]);
          break;
        case SND_COMMAND_START_TIMER:
          SndArm7_StartTimer(command.args[0], command.args[1], command.args[2],
                             command.args[3]);
          break;
        case SND_COMMAND_STOP_TIMER:
          SndArm7_StopTimer(command.args[0], command.args[1], command.args[2],
                            command.args[3]);
          break;
        case SND_COMMAND_SETUP_CAPTURE:
          /* args[2] bits 31 to 27: capture, format, loop, in, out */
          SND_SetupCapture((command.args[2] >> 31) & 1,
                           (command.args[2] >> 30) & 1, (void*)command.args[0],
                           command.args[1], (command.args[2] >> 29) & 1,
                           (command.args[2] >> 28) & 1,
                           (command.args[2] >> 27) & 1);
          break;
        case SND_COMMAND_SETUP_ALARM:
          SndArm7_SetupAlarm(command.args[0], command.args[1], command.args[2],
                             command.args[3]);
          break;
        case SND_COMMAND_CHANNEL_TIMER:
          SndArm7_SetChannelTimer(command.args[0], command.args[1]);
          break;
        case SND_COMMAND_CHANNEL_VOLUME:
          SndArm7_SetChannelVolume(command.args[0], command.args[1],
                                   command.args[2]);
          break;
        case SND_COMMAND_CHANNEL_PAN:
          SndArm7_SetChannelPan(command.args[0], command.args[1]);
          break;
        case SND_COMMAND_SETUP_CHANNEL_PCM:
          /*
                 * args[0]: channel (bits 0-15), timer (16-31); args[1]: sample address;
                 * args[2]: length (0-21), volume shift (22-23), volume (24-30);
                 * args[3]: loop start (0-15), pan (16-22), format (24-25), repeat mode (26-27)
                 */
          SndHw_SetupPcm(
              command.args[0] & 0xFFFF, command.args[1] & 0x7FFFFFF,
              (command.args[3] >> 24) & 3, (command.args[3] >> 26) & 3,
              command.args[3] & 0xFFFF, command.args[2] & 0x3FFFFF,
              (command.args[2] >> 24) & 0x7F, (command.args[2] >> 22) & 3,
              (command.args[0] >> 16) & 0xFFFF, (command.args[3] >> 16) & 0x7F);
          break;
        case SND_COMMAND_SETUP_CHANNEL_PSG:
          /* args[1]: volume (bits 0-6), volume shift (8-9); args[2]: pan (0-6), timer (8-23); args[3]: duty */
          SndHw_SetupPsg(command.args[0], command.args[3],
                         command.args[1] & 0x7F, (command.args[1] >> 8) & 3,
                         (command.args[2] >> 8) & 0xFFFF,
                         command.args[2] & 0x7F);
          break;
        case SND_COMMAND_SETUP_CHANNEL_NOISE:
          /* packed as for SND_COMMAND_SETUP_CHANNEL_PSG */
          SndHw_SetupNoise(command.args[0], command.args[1] & 0x7F,
                           (command.args[1] >> 8) & 3,
                           (command.args[2] >> 8) & 0xFFFF,
                           command.args[2] & 0x7F);
          break;
        case SOUND_REQ_EDGE_PAN_DIP:
          SndHw_SetPanEdgeAttenuation(command.args[0]);
          break;
        case SND_COMMAND_MASTER_VOLUME:
          func_arm7_037fe534(command.args[0]);
          break;
        case SOUND_REQ_FORCE_ALL_PAN:
          SndHw_SetPanOverride(command.args[0]);
          break;
        case SOUND_REQ_MIXER_ROUTING:
          SndHw_SetOutputControl(command.args[0], command.args[1],
                                 command.args[2], command.args[3]);
          break;
        case SND_COMMAND_LOCK_CHANNEL:
          SndExChannel_LockChannels(command.args[0], command.args[1]);
          break;
        case SND_COMMAND_UNLOCK_CHANNEL:
          SndExChannel_UnlockChannels(command.args[0], command.args[1]);
          break;
        case SND_COMMAND_STOP_UNLOCKED_CHANNEL:
          SndExChannel_StopUnlockedChannels(command.args[0], command.args[1]);
          break;
        case SND_COMMAND_INVALIDATE_SEQ:
          SndArm7_InvalidateSeq(command.args[0], command.args[1]);
          break;
        case SND_COMMAND_INVALIDATE_BANK:
          SndArm7_InvalidateBank(command.args[0], command.args[1]);
          break;
        case SND_COMMAND_INVALIDATE_WAVE:
          SndExChannel_InvalidateWave((const void*)command.args[0],
                                      (const void*)command.args[1]);
          break;
        case SND_COMMAND_SHARED_WORK:
          data_arm7_0380808c = (SndSharedWork*)command.args[0];
          break;
        case SOUND_REQ_DUMP_DRIVER_STATE:
          SndArm7_WriteSnapshot((SndArm7Snapshot*)command.args[0]);
          break;
      }
      next = command.next;
    }
    data_arm7_0380808c->finishCommandTag++;
  }
}

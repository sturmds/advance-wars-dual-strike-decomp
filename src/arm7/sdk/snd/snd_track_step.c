#include "sdk/snd.h"
#include "sdk/os_arm7.h"

/* Nonzero when the sequence print-variable command should look up its variable (debug only). */
extern int gSndPrintVarEnabled;

/* Sequence commands. Bytes below 0x80 are notes; their low 7 bits give the key. */
enum {
    SEQ_OP_REST = 0x80,
    SEQ_OP_SET_INSTRUMENT = 0x81,
    SEQ_OP_LAUNCH_TRACK = 0x93,
    SEQ_OP_GOTO = 0x94,
    SEQ_OP_GOSUB = 0x95,
    SEQ_OP_RANDOM_ARG_PREFIX = 0xA0,   /* prefix: the next command's argument is random */
    SEQ_OP_VAR_ARG_PREFIX = 0xA1, /* prefix: the next command's argument is a player variable */
    SEQ_OP_ONLY_IF_TRUE = 0xA2,       /* prefix: run the next command only if the last compare was true */
    SEQ_OP_VAR_ASSIGN = 0xB0,
    SEQ_OP_VAR_PLUS = 0xB1,
    SEQ_OP_VAR_MINUS = 0xB2,
    SEQ_OP_VAR_TIMES = 0xB3,
    SEQ_OP_VAR_DIVIDE = 0xB4,
    SEQ_OP_VAR_BITSHIFT = 0xB5,
    SEQ_OP_VAR_PICK_RANDOM = 0xB6,
    SEQ_OP_TEST_EQUAL = 0xB8,
    SEQ_OP_TEST_AT_LEAST = 0xB9,
    SEQ_OP_TEST_ABOVE = 0xBA,
    SEQ_OP_TEST_AT_MOST = 0xBB,
    SEQ_OP_TEST_BELOW = 0xBC,
    SEQ_OP_TEST_UNEQUAL = 0xBD,
    SEQ_OP_TRACK_BALANCE = 0xC0,
    SEQ_OP_TRACK_LOUDNESS = 0xC1,
    SEQ_OP_PLAYBACK_LOUDNESS = 0xC2,
    SEQ_OP_KEY_OFFSET = 0xC3,
    SEQ_OP_BEND_AMOUNT = 0xC4,
    SEQ_OP_BEND_SPAN = 0xC5,
    SEQ_OP_TRACK_RANK = 0xC6,
    SEQ_OP_BLOCK_ON_NOTES = 0xC7,
    SEQ_OP_LEGATO = 0xC8,
    SEQ_OP_GLIDE_FROM_KEY = 0xC9,
    SEQ_OP_MOD_DEPTH = 0xCA,
    SEQ_OP_MOD_RATE = 0xCB,
    SEQ_OP_MOD_DEST = 0xCC,
    SEQ_OP_MOD_SCALE = 0xCD,
    SEQ_OP_GLIDE_TOGGLE = 0xCE,
    SEQ_OP_GLIDE_DURATION = 0xCF,
    SEQ_OP_ENV_RISE_RATE = 0xD0,
    SEQ_OP_ENV_FALL_RATE = 0xD1,
    SEQ_OP_ENV_HOLD_LEVEL = 0xD2,
    SEQ_OP_ENV_FADE_RATE = 0xD3,
    SEQ_OP_REPEAT_BEGIN = 0xD4,
    SEQ_OP_TRACK_EXPRESSION = 0xD5,
    SEQ_OP_DEBUG_SHOW_VAR = 0xD6,
    SEQ_OP_MOD_ONSET_DELAY = 0xE0,
    SEQ_OP_BEATS_PER_MINUTE = 0xE1,
    SEQ_OP_PITCH_SLIDE_START = 0xE3,
    SEQ_OP_REPEAT_CLOSE = 0xFC,
    SEQ_OP_BACK_FROM_GOSUB = 0xFD,
    SEQ_OP_RESERVE_TRACKS = 0xFE,
    SEQ_OP_FINISH_TRACK = 0xFF
};

#define SEQ_NESTING_DEPTH 3

/* Runs one tick of a track: counts down its channels and its wait, then reads and runs commands until it has to wait again. play is FALSE when skipping ahead without starting notes. Returns -1 at the end of the track, else 0. */
int SndTrack_Step(SndTrack *track, SndPlayer *player, int trackNo, int play)
{
    SndExChannel *channel;
    int cmd;
    BOOL run;
    BOOL override;
    int argType;

    for (channel = track->channels; channel != NULL; channel = channel->next) {
        if (channel->length > 0) {
            channel->length--;
        }
        if (!channel->autoSweep && channel->sweepCounter < channel->sweepLength) {
            channel->sweepCounter++;
        }
    }

    if (track->noteFinished) {
        if (track->channels != NULL) {
            return 0;
        }
        track->noteFinished = FALSE;
    }

    if (track->wait > 0) {
        track->wait--;
        if (track->wait > 0) {
            return 0;
        }
    }

    SndSeq_FillCache(track->cursor);

    while (track->wait == 0 && !track->noteFinished) {
        override = FALSE;
        run = TRUE;

        cmd = SndSeq_ReadByte(track->cursor);
        track->cursor++;

        if (cmd == SEQ_OP_ONLY_IF_TRUE) {
            cmd = SndSeq_ReadByte(track->cursor);
            track->cursor++;
            run = track->compare;
        }
        if (cmd == SEQ_OP_RANDOM_ARG_PREFIX) {
            cmd = SndSeq_ReadByte(track->cursor);
            track->cursor++;
            argType = SEQ_ARG_FROM_RANGE;
            override = TRUE;
        }
        if (cmd == SEQ_OP_VAR_ARG_PREFIX) {
            cmd = SndSeq_ReadByte(track->cursor);
            track->cursor++;
            argType = SEQ_ARG_FROM_PLAYER_VAR;
            override = TRUE;
        }

        if ((cmd & 0x80) == 0) {
            u8 velocity;
            s32 length;
            int key;

            velocity = SndSeq_ReadByte(track->cursor);
            track->cursor++;
            length = SndTrack_ReadArg(track, player, override ? argType : SEQ_ARG_FROM_VARINT);
            key = cmd + track->transpose;
            if (!run) {
                continue;
            }
            if (key < 0) {
                key = 0;
            } else if (key > 127) {
                key = 127;
            }
            if (!track->mute && play) {
                SndTrack_NoteOn(track, player, key, velocity, length > 0 ? length : -1);
            }
            track->portamentoKey = key;
            if (track->noteWait) {
                track->wait = length;
                if (length == 0) {
                    track->noteFinished = TRUE;
                }
            }
            continue;
        }

        switch (cmd & 0xF0) {
        case 0x80: {
            int arg = SndTrack_ReadArg(track, player, override ? argType : SEQ_ARG_FROM_VARINT);
            if (!run) {
                break;
            }
            switch (cmd) {
            case SEQ_OP_REST:
                track->wait = arg;
                break;
            case SEQ_OP_SET_INSTRUMENT:
                if (arg < 0x10000) {
                    track->program = arg;
                }
                break;
            }
            break;
        }
        case 0x90:
            switch (cmd) {
            case SEQ_OP_LAUNCH_TRACK: {
                u8 number;
                u32 offset;
                SndTrack *other;

                number = SndSeq_ReadByte(track->cursor);
                track->cursor++;
                offset = SndTrack_ReadU24(track);
                if (run) {
                    other = SndPlayer_GetTrack(player, number);
                    if (other != NULL && other != track) {
                        SndTrack_Close(other, player);
                        SndTrack_Start(other, track->base, offset);
                    }
                }
                break;
            }
            case SEQ_OP_GOTO: {
                u32 offset = SndTrack_ReadU24(track);
                if (run) {
                    track->cursor = track->base + offset;
                }
                break;
            }
            case SEQ_OP_GOSUB: {
                u32 offset = SndTrack_ReadU24(track);
                if (run && track->callDepth < SEQ_NESTING_DEPTH) {
                    track->callStack[track->callDepth] = track->cursor;
                    track->callDepth++;
                    track->cursor = track->base + offset;
                }
                break;
            }
            }
            break;
        case 0xC0:
        case 0xD0: {
            /* Needed to match: the cases below are in the original's block order, and the signed fields read arg through an s8 view, which keeps it on the stack. */
            u8 arg = SndTrack_ReadArg(track, player, override ? argType : SEQ_ARG_FROM_U8);
            if (!run) {
                break;
            }
            switch (cmd) {
            case SEQ_OP_TRACK_LOUDNESS:
                track->volume = arg;
                break;
            case SEQ_OP_TRACK_EXPRESSION:
                track->volume2 = arg;
                break;
            case SEQ_OP_PLAYBACK_LOUDNESS:
                player->volume = arg;
                break;
            case SEQ_OP_BEND_SPAN:
                track->bendRange = arg;
                break;
            case SEQ_OP_TRACK_RANK:
                track->priority = arg;
                break;
            case SEQ_OP_BLOCK_ON_NOTES:
                track->noteWait = arg;
                break;
            case SEQ_OP_GLIDE_DURATION:
                track->portamentoTime = arg;
                break;
            case SEQ_OP_MOD_DEPTH:
                track->lfo.depth = arg;
                break;
            case SEQ_OP_MOD_RATE:
                track->lfo.speed = arg;
                break;
            case SEQ_OP_MOD_DEST:
                track->lfo.target = arg;
                break;
            case SEQ_OP_MOD_SCALE:
                track->lfo.range = arg;
                break;
            case SEQ_OP_ENV_RISE_RATE:
                track->attack = arg;
                break;
            case SEQ_OP_ENV_FALL_RATE:
                track->decay = arg;
                break;
            case SEQ_OP_ENV_HOLD_LEVEL:
                track->sustain = arg;
                break;
            case SEQ_OP_ENV_FADE_RATE:
                track->release = arg;
                break;
            case SEQ_OP_REPEAT_BEGIN:
                if (track->callDepth < SEQ_NESTING_DEPTH) {
                    track->callStack[track->callDepth] = track->cursor;
                    track->loopCount[track->callDepth] = arg;
                    track->callDepth++;
                }
                break;
            case SEQ_OP_LEGATO:
                track->tie = arg;
                SndTrack_ReleaseChannels(track, player, -1);
                SndTrack_FreeChannels(track);
                break;
            case SEQ_OP_GLIDE_FROM_KEY:
                track->portamentoKey = arg + track->transpose;
                track->portamento = TRUE;
                break;
            case SEQ_OP_GLIDE_TOGGLE:
                track->portamento = arg;
                break;
            case SEQ_OP_KEY_OFFSET:
                track->transpose = *(s8 *)&arg;
                break;
            case SEQ_OP_BEND_AMOUNT:
                track->pitchBend = *(s8 *)&arg;
                break;
            case SEQ_OP_TRACK_BALANCE:
                track->pan = arg - 64;
                break;
            case SEQ_OP_DEBUG_SHOW_VAR:
                if (gSndPrintVarEnabled) {
                    SndPlayer_GetVariablePtr(player, arg);
                }
                break;
            }
            break;
        }
        case 0xE0: {
            s16 arg = SndTrack_ReadArg(track, player, override ? argType : SEQ_ARG_FROM_U16);
            if (!run) {
                break;
            }
            switch (cmd) {
            case SEQ_OP_BEATS_PER_MINUTE:
                player->tempo = arg;
                break;
            case SEQ_OP_MOD_ONSET_DELAY:
                track->lfo.delay = arg;
                break;
            case SEQ_OP_PITCH_SLIDE_START:
                track->sweepPitch = arg;
                break;
            }
            break;
        }
        case 0xB0: {
            u8 index;
            s16 arg;
            s16 *variable;

            index = SndSeq_ReadByte(track->cursor);
            track->cursor++;
            arg = SndTrack_ReadArg(track, player, override ? argType : SEQ_ARG_FROM_U16);
            variable = SndPlayer_GetVariablePtr(player, index);
            if (!run || variable == NULL) {
                break;
            }
            switch (cmd) {
            case SEQ_OP_VAR_ASSIGN:
                *variable = arg;
                break;
            case SEQ_OP_VAR_PLUS:
                *variable += arg;
                break;
            case SEQ_OP_VAR_MINUS:
                *variable -= arg;
                break;
            case SEQ_OP_VAR_TIMES:
                *variable *= arg;
                break;
            case SEQ_OP_VAR_DIVIDE:
                if (arg != 0) {
                    *variable = _s32_div_f(*variable, arg);
                }
                break;
            case SEQ_OP_VAR_BITSHIFT:
                if (arg >= 0) {
                    *variable <<= arg;
                } else {
                    *variable >>= -arg;
                }
                break;
            case SEQ_OP_VAR_PICK_RANDOM: {
                BOOL negative = FALSE;
                s32 random;

                if (arg < 0) {
                    negative = TRUE;
                    arg = -arg;
                }
                random = SndDrv_NextRandom();
                random = (random * (arg + 1)) >> 16;
                if (negative) {
                    random = -random;
                }
                *variable = random;
                break;
            }
            case SEQ_OP_TEST_EQUAL:
                track->compare = *variable == arg;
                break;
            case SEQ_OP_TEST_AT_LEAST:
                track->compare = *variable >= arg;
                break;
            case SEQ_OP_TEST_ABOVE:
                track->compare = *variable > arg;
                break;
            case SEQ_OP_TEST_AT_MOST:
                track->compare = *variable <= arg;
                break;
            case SEQ_OP_TEST_BELOW:
                track->compare = *variable < arg;
                break;
            case SEQ_OP_TEST_UNEQUAL:
                track->compare = *variable != arg;
                break;
            }
            break;
        }
        case 0xF0:
            if (!run) {
                break;
            }
            switch (cmd) {
            case SEQ_OP_BACK_FROM_GOSUB:
                if (track->callDepth == 0) {
                    break;
                }
                track->callDepth--;
                track->cursor = track->callStack[track->callDepth];
                break;
            case SEQ_OP_REPEAT_CLOSE: {
                u8 depth = track->callDepth;
                u8 count;

                if (depth == 0) {
                    break;
                }
                count = track->loopCount[depth - 1];
                if (count != 0) {
                    count--;
                    if (count == 0) {
                        track->callDepth = depth - 1;
                        break;
                    }
                }
                track->loopCount[depth - 1] = count;
                track->cursor = track->callStack[track->callDepth - 1];
                break;
            }
            case SEQ_OP_RESERVE_TRACKS:
                break;
            case SEQ_OP_FINISH_TRACK:
                return -1;
            }
            break;
        }
    }
    return 0;
}

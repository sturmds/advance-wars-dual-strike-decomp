#!/usr/bin/env python3
"""GBA MusicPlayer2000 (m4a) songs: list them, render them, import one as DS sound files.

    m4a.py list ROM                              every song: tracks, voicegroup, player, size
    m4a.py render ROM SONG [--out FILE.wav] [--loops 2] [--fade 8]
                                                 a rough software render, to identify a song by ear
    m4a.py import ROM SONG --name NAME [--like SEQ] [--volume N] [--sound DIR]
                                                 the song as an SSEQ, SBNK and SWAR in the unpacked
                                                 sound archive, with its sound.yaml entries

Many GBA games play their music with Nintendo's MusicPlayer2000 sound
driver, also known as m4a or Sappy. A song is a header (track count,
priority, reverb, voicegroup) and one byte stream of commands per track; a
voicegroup is an array of 12-byte voices (DirectSound samples, the four GB
channels, key splits and drum kits). See docs/m4a.md for the formats and how
the import maps each part to the DS sound formats (docs/sound.md).

The m4a driver's behaviour is taken from pret's pokeemerald decompilation
(docs/third-party/pokeemerald.md), the DS driver's from our own ARM7 code,
cross-checked with pokediamond and SSEQPlayer (docs/third-party/
pokediamond.md and sseqplayer-caitsith2.md). Finding the song table through
m4aSongNumStart is the method of Sappy and GBA Mus Riper
(docs/third-party/gba-mus-ripper.md).

The ROM belongs to the builder: nothing read from it enters git
(tenet 9). `import` writes into extracted/AWRE/assets/sound/, which is
gitignored, and the build packs it into the ROM's sound archive.
"""

import argparse
import bisect
import math
import random
import re
import struct
import sys
import wave
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GBA_ROM_BASE = 0x08000000

# The first 30 bytes of the driver's song select function, m4aSongNumStart:
# push {lr}; lsl r0, r0, 16; ldr r2, =gMPlayTable; ldr r1, =gSongTable; ...
# bl MPlayStart. The word 40 bytes after its start holds the song table's
# address (docs/m4a.md, "Finding the song table").
SELECT_SONG = bytes.fromhex("00 b5 00 04 07 4a 08 49 40 0b 40 18 83 88 59 00"
                            " c9 18 89 00 89 18 0a 68 01 68 10 1c 00 f0")
SONG_TABLE_POINTER = 40

# Note and wait lengths in ticks, indexed by command - 0x80 (waits) or
# command - 0xcf (notes; 0xcf, TIE, has length 0).
CLOCKS = (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
          28, 30, 32, 36, 40, 42, 44, 48, 52, 54, 56, 60, 64, 66, 68, 72, 76, 78, 80, 84, 88, 90, 92, 96)
TICKS_PER_QUARTER = 24

# Command bytes.
FINE, GOTO, PATT, PEND, REPT = 0xb1, 0xb2, 0xb3, 0xb4, 0xb5
MEMACC, PRIO, TEMPO, KEYSH = 0xb9, 0xba, 0xbb, 0xbc
VOICE, VOL, PAN, BEND, BENDR, LFOS, LFODL, MOD, MODT = 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5
TUNE, PORT, XCMD, EOT, TIE = 0xc8, 0xcc, 0xcd, 0xce, 0xcf
# Commands with one argument byte, by name.
ONE_ARG = {PRIO: "prio", TEMPO: "tempo", KEYSH: "keysh", VOICE: "voice", VOL: "vol", PAN: "pan",
           BEND: "bend", BENDR: "bendr", LFOS: "lfos", LFODL: "lfodl", MOD: "mod", MODT: "modt",
           TUNE: "tune"}
# Arguments stored around 0x40 (c_v): pan, bend and tune are signed.
CENTRED = {"pan", "bend", "tune"}

# Voice types (the low bits of ToneData.type).
TYPE_CGB = 0x07        # 1, 2 square, 3 wave, 4 noise; 0 DirectSound
TYPE_FIXED = 0x08      # DirectSound at the mixing rate, whatever the key
TYPE_SPLIT = 0x40
TYPE_DRUMS = 0x80
WAVE_LOOP = 0x4000     # WaveData.status


def u32(data: bytes, off: int) -> int:
    return struct.unpack_from("<I", data, off)[0]


class Rom:
    """A GBA ROM image, addressed by bus address (0x08000000 up)."""

    def __init__(self, path: Path) -> None:
        self.path = path
        self.data = path.read_bytes()
        if len(self.data) < 0xc0 or self.data[0xb2] != 0x96:
            sys.exit(f"error: {path} is not a GBA ROM (no 0x96 at 0xb2)")

    @property
    def title(self) -> str:
        return self.data[0xa0:0xb0].decode("ascii", "replace").rstrip("\0")

    def valid(self, addr: int) -> bool:
        return GBA_ROM_BASE <= addr < GBA_ROM_BASE + len(self.data)

    def off(self, addr: int) -> int:
        if not self.valid(addr):
            raise ValueError(f"address {addr:#x} is outside the ROM")
        return addr - GBA_ROM_BASE

    def byte(self, addr: int) -> int:
        return self.data[self.off(addr)]

    def word(self, addr: int) -> int:
        return u32(self.data, self.off(addr))

    def bytes(self, addr: int, n: int) -> bytes:
        o = self.off(addr)
        return self.data[o:o + n]


# ---------------------------------------------------------------- songs

@dataclass
class Song:
    index: int
    header: int            # address of the song header
    player: int            # music player (ms)
    tracks: list[int]      # address of each track's commands
    priority: int
    reverb: int
    voicegroup: int


def song_table(rom: Rom) -> tuple[int, int]:
    """Address of the song table and its number of songs."""
    at = rom.data.find(SELECT_SONG)
    if at < 0:
        sys.exit(f"error: {rom.path}: no MusicPlayer2000 driver found (m4aSongNumStart's code)")
    table = u32(rom.data, at + SONG_TABLE_POINTER)
    if not rom.valid(table):
        sys.exit(f"error: {rom.path}: the song table pointer {table:#x} is outside the ROM")
    # The table ends at the first entry whose header is not a ROM address.
    n = 0
    while rom.valid(table + 8 * n + 8) and rom.valid(rom.word(table + 8 * n)):
        n += 1
    return table, n


def read_song(rom: Rom, table: int, index: int) -> Song:
    header, player, _ = struct.unpack("<IHH", rom.bytes(table + 8 * index, 8))
    count, _, priority, reverb, voicegroup = struct.unpack("<BBBBI", rom.bytes(header, 8))
    tracks = list(struct.unpack(f"<{count}I", rom.bytes(header + 8, 4 * count)))
    return Song(index, header, player, tracks, priority, reverb, voicegroup)


def songs(rom: Rom) -> list[Song]:
    table, n = song_table(rom)
    return [read_song(rom, table, i) for i in range(n)]


def get_song(rom: Rom, index: int) -> Song:
    table, n = song_table(rom)
    if not 0 <= index < n:
        sys.exit(f"error: song {index} out of range; the table has {n} songs")
    song = read_song(rom, table, index)
    if not song.tracks:
        sys.exit(f"error: song {index} has no tracks")
    return song


# m4aSoundMode's argument as m4aSoundInit passes it, a constant just before
# m4aSongNumStart: DA bits (8-11) at bit 20, mixing rate index at bit 16,
# master volume at bit 12, DirectSound channels at bit 8.
MIX_RATES = (0, 5734, 7884, 10512, 13379, 15768, 18157, 21024, 26758, 31536, 36314, 40137, 42048)
DEFAULT_MIX_RATE = 13379


def mix_rate(rom: Rom) -> tuple[int, bool]:
    """The DirectSound mixing rate in Hz, and whether it was found in the ROM.
    Fixed-pitch voices (type 0x08) play at this rate whatever the key."""
    at = rom.data.find(SELECT_SONG)
    for o in range(at - 4, max(0, at - 0x100), -4):
        w = u32(rom.data, o)
        if (w >> 24 == 0 and w & 0xff == 0 and 8 <= (w >> 20 & 15) <= 11 and 1 <= (w >> 16 & 15) <= 12
                and w >> 12 & 15 and 1 <= (w >> 8 & 15) <= 12):
            return MIX_RATES[w >> 16 & 15], True
    return DEFAULT_MIX_RATE, False


# ---------------------------------------------------------------- track commands

@dataclass
class Event:
    """One decoded command, its arguments resolved (running status, omitted
    key and velocity), as the driver runs it at `pc`."""
    pc: int
    size: int
    kind: str               # wait, note, eot, fine, goto, patt, pend, rept, memacc, xcmd, or a ONE_ARG name
    args: tuple = ()


@dataclass
class TrackState:
    """What the driver keeps per track while decoding."""
    running: int = 0        # last command 0xbd or above, repeated by a byte below 0x80
    key: int = 60
    velocity: int = 127


def decode(rom: Rom, pc: int, st: TrackState) -> Event:
    """The command at pc, updating the running status, key and velocity."""
    cmd = rom.byte(pc)
    p = pc + 1
    if cmd < 0x80:                     # running status: this byte gives the first argument
        cmd, p = st.running, pc
        if cmd < VOICE:
            raise ValueError(f"{pc:#x}: argument byte {rom.byte(pc):#x} with no command to repeat")
    elif cmd >= VOICE:
        st.running = cmd
    if cmd <= 0xb0:
        return Event(pc, p - pc, "wait", (CLOCKS[cmd - 0x80],))
    if cmd >= TIE:
        # Key, velocity and extra gate ticks, each optional (a byte below 0x80).
        length = CLOCKS[cmd - TIE]
        gate = 0
        if rom.byte(p) < 0x80:
            st.key = rom.byte(p)
            p += 1
            if rom.byte(p) < 0x80:
                st.velocity = rom.byte(p)
                p += 1
                if rom.byte(p) < 0x80:
                    gate = rom.byte(p)
                    p += 1
        # Length 0 (TIE) plays until the EOT for its key.
        return Event(pc, p - pc, "note", (st.key, st.velocity, length + gate if length else 0))
    if cmd == EOT:
        if rom.byte(p) < 0x80:
            st.key = rom.byte(p)
            p += 1
        return Event(pc, p - pc, "eot", (st.key,))
    if cmd in ONE_ARG:
        name, v = ONE_ARG[cmd], rom.byte(p)
        if name in CENTRED:
            v -= 0x40
        elif name == "keysh":
            v = v - 256 if v >= 0x80 else v
        elif name == "tempo":
            v *= 2                     # the argument is half the tempo in BPM
        return Event(pc, p + 1 - pc, name, (v,))
    if cmd == FINE:
        return Event(pc, 1, "fine")
    if cmd in (GOTO, PATT):
        return Event(pc, 5, "goto" if cmd == GOTO else "patt", (rom.word(p),))
    if cmd == PEND:
        return Event(pc, 1, "pend")
    if cmd == REPT:
        return Event(pc, 6, "rept", (rom.byte(p), rom.word(p + 1)))
    if cmd == MEMACC:
        return Event(pc, 4, "memacc", tuple(rom.bytes(p, 3)))
    if cmd == PORT:
        return Event(pc, 3, "port", tuple(rom.bytes(p, 2)))
    if cmd == XCMD:
        # Extended commands: 0 sets the voice's wave (4 bytes), the rest take one byte.
        n = rom.byte(p)
        size = 4 if n in (0x00, 0x0d) else 1
        return Event(pc, 2 + size, "xcmd", (n, rom.bytes(p + 1, size)))
    raise ValueError(f"{pc:#x}: unknown command {cmd:#x}")


@dataclass
class Played:
    """One event as a track runs it, at tick `time`."""
    time: int
    event: Event
    length: int = 0         # a note's length in ticks, ties resolved
    ended_by: int | None = None   # a TIE: the EOT that ended it


@dataclass
class TrackRun:
    played: list[Played]
    loop_start: int | None  # tick of the GOTO target on the first pass, None if the track ends
    loop_end: int           # tick of the GOTO (or FINE)
    loop_pc: int | None
    warnings: list[str] = field(default_factory=list)


MAX_TICKS = 24 * 4 * 2000  # stops a track that never reaches FINE or a backward GOTO


def run_track(rom: Rom, start: int, loops: int = 2) -> TrackRun:
    """Run one track the way the driver does, following GOTO `loops` times.

    A TIE lasts until the EOT that ends it: the newest open tie
    with that key, as the driver stops the newest channel with the key.
    """
    st = TrackState()
    pc, stack, rep = start, [], 0
    time = 0
    played: list[Played] = []
    open_ties: list[Played] = []
    warnings: list[str] = []
    gotos = 0
    loop_start = loop_pc = None
    goto_pcs: dict[int, int] = {}      # GOTO target -> tick of its first visit
    first_visit: dict[int, int] = {}
    while time < MAX_TICKS:
        ev = decode(rom, pc, st)
        first_visit.setdefault(pc, time)
        p = Played(time, ev)
        played.append(p)
        nxt = pc + ev.size
        if ev.kind == "wait":
            time += ev.args[0]
        elif ev.kind == "note":
            p.length = ev.args[2]
            if p.length == 0:
                open_ties.append(p)
        elif ev.kind == "eot":
            for t in reversed(open_ties):
                if t.event.args[0] == ev.args[0]:
                    t.length = time - t.time
                    t.ended_by = ev.pc
                    open_ties.remove(t)
                    break
        elif ev.kind == "fine":
            break
        elif ev.kind == "goto":
            target = ev.args[0]
            if target in first_visit and loop_start is None:
                loop_start, loop_pc = first_visit[target], target
                goto_pcs[target] = time
            gotos += 1
            if gotos >= loops + 1 or (gotos >= loops and loop_start is not None):
                break
            nxt = target
        elif ev.kind == "patt":
            if len(stack) >= 3:
                raise ValueError(f"{pc:#x}: patterns nested more than 3 deep")
            stack.append(nxt)
            nxt = ev.args[0]
        elif ev.kind == "pend":
            if stack:
                nxt = stack.pop()
        elif ev.kind == "rept":
            count, target = ev.args
            if count == 0:
                nxt = target
            else:
                rep += 1
                if rep < count:
                    nxt = target
                else:
                    rep = 0
        pc = nxt
    else:
        warnings.append(f"track at {start:#x} ran {MAX_TICKS} ticks without ending; cut there")
    for t in open_ties:
        t.length = max(1, time - t.time)
        warnings.append(f"track at {start:#x}: TIE at {t.event.pc:#x} has no EOT; held to the end")
    return TrackRun(played, loop_start, time, loop_pc, warnings)


def track_size(rom: Rom, start: int) -> int:
    """Bytes of commands the track reaches (patterns counted once)."""
    seen: set[int] = set()
    todo = [start]
    total = 0
    while todo:
        pc = todo.pop()
        st = TrackState()
        while pc not in seen:
            seen.add(pc)
            try:
                ev = decode(rom, pc, st)
            except (ValueError, IndexError):
                break
            total += ev.size
            if ev.kind in ("fine", "pend"):
                break
            if ev.kind == "goto":
                todo.append(ev.args[0])
                break
            if ev.kind in ("patt", "rept"):
                todo.append(ev.args[-1])
            pc += ev.size
    return total


# ---------------------------------------------------------------- voicegroups

@dataclass
class Sample:
    addr: int
    rate: float             # Hz at key 60 (WaveData.freq / 1024)
    loop: bool
    loop_start: int
    data: bytes             # signed 8-bit PCM


@dataclass
class Voice:
    """One 12-byte ToneData."""
    addr: int
    type: int
    key: int
    length: int
    pan_sweep: int
    wav: int                # sample, wave pattern, duty or noise period, or sub-voicegroup
    attack: int
    decay: int
    sustain: int
    release: int
    keymap: int = 0         # key split: address of the 128-byte key -> voice table

    @property
    def kind(self) -> str:
        if self.type & TYPE_SPLIT:
            return "split"
        if self.type & TYPE_DRUMS:
            return "drums"
        return ("direct", "square1", "square2", "wave", "noise", "square1", "square2", "wave")[self.type & TYPE_CGB]


def read_voice(rom: Rom, addr: int) -> Voice:
    t, key, length, ps, wav, a, d, s, r = struct.unpack("<BBBBIBBBB", rom.bytes(addr, 12))
    v = Voice(addr, t, key, length, ps, wav, a, d, s, r)
    if t & TYPE_SPLIT:
        v.keymap = u32(rom.bytes(addr + 8, 4), 0)
    return v


def sub_voice(rom: Rom, v: Voice, key: int) -> Voice:
    """The voice a key split or drum kit plays for `key`."""
    index = rom.byte(v.keymap + key) if v.type & TYPE_SPLIT else key
    return read_voice(rom, v.wav + 12 * index)


def read_sample(rom: Rom, addr: int) -> Sample:
    kind, status, freq, loop_start, size = struct.unpack("<HHIII", rom.bytes(addr, 16))
    if kind != 0:
        raise ValueError(f"sample at {addr:#x}: type {kind}, not 8-bit PCM")
    return Sample(addr, freq / 1024, bool(status & WAVE_LOOP), loop_start, rom.bytes(addr + 16, size))


def wave_pattern(rom: Rom, addr: int) -> list[int]:
    """The GB wave channel's 32 4-bit steps, high nibble first."""
    raw = rom.bytes(addr, 16)
    return [n for b in raw for n in (b >> 4, b & 15)]


# ---------------------------------------------------------------- list

def cmd_list(args) -> int:
    rom = Rom(Path(args.rom))
    table, n = song_table(rom)
    print(f"{rom.path.name} ({rom.title}): song table at {table:#x}, {n} songs")
    print(f"{'song':>5} {'player':>6} {'tracks':>6} {'prio':>4} {'voicegroup':>10} {'bytes':>6}")
    for s in songs(rom):
        if not s.tracks and not args.all:
            continue
        size = sum(track_size(rom, t) for t in s.tracks)
        print(f"{s.index:5} {s.player:6} {len(s.tracks):6} {s.priority:4} {s.voicegroup:#10x} {size:6}")
    return 0


# ---------------------------------------------------------------- render

RATE = 22050               # output sample rate
FRAME = 60                 # the driver updates envelopes once a frame (VBlank)
CGB_LEVEL = 0.25           # a GB channel at volume 15 against a full DirectSound channel (rough)


def tempo_map(runs: list[TrackRun]) -> list[tuple[int, int]]:
    """(tick, bpm) changes, from TEMPO commands of any track."""
    changes = sorted((p.time, p.event.args[0]) for r in runs for p in r.played if p.event.kind == "tempo")
    out = [(0, 150)]
    for t, bpm in changes:
        if out[-1][0] == t:
            out[-1] = (t, bpm)
        else:
            out.append((t, bpm))
    return out


class Clock:
    """Ticks to seconds through the tempo map: 24 ticks per quarter note."""

    def __init__(self, tempos: list[tuple[int, int]]) -> None:
        self.ticks = [t for t, _ in tempos]
        self.bpm = [b for _, b in tempos]
        self.secs = [0.0]
        for i in range(1, len(tempos)):
            self.secs.append(self.secs[-1] + (self.ticks[i] - self.ticks[i - 1]) * self.tick_len(i - 1))

    def tick_len(self, i: int) -> float:
        return 60 / (max(self.bpm[i], 1) * TICKS_PER_QUARTER)

    def seconds(self, tick: float) -> float:
        i = bisect.bisect_right(self.ticks, tick) - 1
        return self.secs[i] + (tick - self.ticks[i]) * self.tick_len(i)

    def bpm_at(self, tick: float) -> int:
        return self.bpm[bisect.bisect_right(self.ticks, tick) - 1]


class Controls:
    """A track's controller values over time (vol, pan, bend...), for the render."""

    DEFAULTS = {"vol": 100, "pan": 0, "bend": 0, "bendr": 2, "tune": 0, "keysh": 0, "mod": 0,
                "modt": 0, "lfos": 22, "lfodl": 0}

    def __init__(self, run: TrackRun) -> None:
        self.times: dict[str, list[int]] = {k: [0] for k in self.DEFAULTS}
        self.values: dict[str, list[int]] = {k: [v] for k, v in self.DEFAULTS.items()}
        for p in run.played:
            k = p.event.kind
            if k in self.times:
                self.times[k].append(p.time)
                self.values[k].append(p.event.args[0])

    def at(self, name: str, tick: float) -> int:
        return self.values[name][bisect.bisect_right(self.times[name], tick) - 1]


def envelope_direct(v: Voice, frames_on: int, total_frames: int) -> list[float]:
    """DirectSound envelope per frame (0 to 1): attack adds, decay and release multiply."""
    env, out, phase = 0, [], "attack"
    for f in range(total_frames):
        if f == frames_on:
            phase = "release"
        if phase == "attack":
            env += v.attack
            if env >= 255:
                env, phase = 255, "decay"
        elif phase == "decay":
            env = env * v.decay >> 8
            if env <= v.sustain:
                env, phase = v.sustain, "sustain"
        elif phase == "release":
            env = env * v.release >> 8
        out.append(env / 255)
        if phase == "release" and env == 0:
            break
    return out


def envelope_cgb(v: Voice, goal: int, frames_on: int, total_frames: int) -> list[float]:
    """GB channel envelope per frame (0 to 1): one step of 1/15 every
    attack, decay or release frames, as the driver's CgbSound steps it."""
    sustain_goal = (goal * v.sustain + 15) >> 4
    a, d, r = v.attack & 7, v.decay & 7, v.release & 7
    level = 0 if a else goal
    phase = "attack" if a else "decay"
    counter = a if a else d
    if phase == "decay" and not d:
        level, phase = sustain_goal, "sustain"
    out = []
    for f in range(total_frames):
        if f == frames_on:
            if not r:
                break
            phase, counter = "release", r
        counter -= 1
        if counter <= 0:
            if phase == "attack":
                level += 1
                counter = a
                if level >= goal:
                    level, phase, counter = goal, "decay", d
                    if not d:
                        level, phase = sustain_goal, "sustain"
            elif phase == "decay":
                level -= 1
                counter = d
                if level <= sustain_goal:
                    level, phase = sustain_goal, "sustain"
            elif phase == "release":
                level -= 1
                counter = r
                if level <= 0:
                    break
        out.append(level / 15)
    return out


def note_pitch(ctl: Controls, tick: float, key: int, lfo: float) -> float:
    """Pitch in semitones: key, KEYSH, BEND x BENDR, TUNE and the LFO, as TrkVolPitSet adds them."""
    bend = ctl.at("bend", tick) * ctl.at("bendr", tick) + ctl.at("tune", tick)
    return key + ctl.at("keysh", tick) + bend / 64 + lfo


def render_song(rom: Rom, song: Song, loops: int, fade: float) -> tuple[list[float], list[float], float]:
    runs = [run_track(rom, t, loops) for t in song.tracks]
    fixed_rate = mix_rate(rom)[0]
    clock = Clock(tempo_map(runs))
    end_tick = max(r.loop_end for r in runs)
    seconds = clock.seconds(end_tick) + fade
    n = int(seconds * RATE) + RATE
    left = [0.0] * n
    right = [0.0] * n
    samples: dict[int, Sample] = {}
    rng = random.Random(0)
    for run in runs:
        ctl = Controls(run)
        voice_no = 0
        for p in run.played:
            ev = p.event
            if ev.kind == "voice":
                voice_no = ev.args[0]
                continue
            if ev.kind != "note":
                continue
            key, velocity, length = ev.args[0], ev.args[1], p.length
            v = read_voice(rom, song.voicegroup + 12 * voice_no)
            pan_extra = 0
            if v.type & (TYPE_SPLIT | TYPE_DRUMS):
                drums = bool(v.type & TYPE_DRUMS)
                v = sub_voice(rom, v, key)
                if drums:
                    key = v.key
                    if v.pan_sweep & 0x80:
                        pan_extra = (v.pan_sweep - 0xc0) * 2
            if v.type & (TYPE_SPLIT | TYPE_DRUMS):
                continue
            t0 = clock.seconds(p.time)
            t_off = clock.seconds(p.time + length)
            frames_on = max(1, round((t_off - t0) * FRAME))
            vol = ctl.at("vol", p.time)
            y = max(-128, min(127, 2 * ctl.at("pan", p.time) + pan_extra))
            gain_r = (y + 128) / 256 * 2 * vol / 127 * velocity / 127
            gain_l = (127 - y) / 256 * 2 * vol / 127 * velocity / 127
            kind = v.kind
            if kind == "direct":
                if v.wav not in samples:
                    samples[v.wav] = read_sample(rom, v.wav)
                smp = samples[v.wav]
                env = envelope_direct(v, frames_on, frames_on + 10 * FRAME)
                level = 1 / 128
            else:
                goal = min(15, (velocity * vol * 2 // 128) >> 4)
                env = envelope_cgb(v, goal, frames_on, frames_on + 10 * FRAME)
                level = CGB_LEVEL / 8
                gain_l, gain_r = (2 * vol / 127, 2 * vol / 127) if y == 0 else (
                    (0, 2 * vol / 127) if y > 0 else (2 * vol / 127, 0))
                gain_l /= 2
                gain_r /= 2
            pos = 0.0
            phase = 0.0
            noise_val = 1.0
            lfo_phase = 0
            start = int(t0 * RATE)
            per = RATE // FRAME
            for f, e in enumerate(env):
                a = start + f * per
                if a + per > n:
                    break
                if e <= 0:
                    continue
                tick = p.time + (f / FRAME) / clock.tick_len(bisect.bisect_right(clock.ticks, p.time) - 1)
                mod = ctl.at("mod", tick)
                lfo = 0.0
                if mod and ctl.at("modt", tick) == 0:
                    lfo_phase = (lfo_phase + ctl.at("lfos", tick) * clock.bpm_at(tick) * 0.4 / FRAME) % 256
                    c = int(lfo_phase)
                    tri = c - 256 if c >= 192 else (c if c < 64 else 128 - c)
                    lfo = (mod * tri >> 6) * 16 / 256
                pitch = note_pitch(ctl, tick, key, lfo)
                if kind == "direct":
                    if v.type & TYPE_FIXED:
                        step = fixed_rate / RATE
                    else:
                        step = smp.rate * 2 ** ((pitch - 60) / 12) / RATE
                    data, size = smp.data, len(smp.data)
                    if not size:
                        break
                    chunk = []
                    for i in range(per):
                        ip = int(pos)
                        if ip >= size:
                            if not smp.loop or smp.loop_start >= size:
                                break
                            pos = smp.loop_start + (pos - smp.loop_start) % (size - smp.loop_start)
                            ip = int(pos)
                        s = data[ip]
                        chunk.append((s - 256 if s >= 128 else s) * e * level)
                        pos += step
                    if not chunk:
                        break
                else:
                    freq = 440 * 2 ** ((pitch - 69) / 12)
                    if kind == "wave":
                        freq /= 2         # the wave channel plays an octave below the square channels
                        steps = wave_pattern(rom, v.wav)
                    duty = (0.125, 0.25, 0.5, 0.75)[v.wav & 3] if kind.startswith("square") else 0
                    inc = freq / RATE
                    chunk = []
                    for i in range(per):
                        if kind == "wave":
                            s = (steps[int(phase * 32) & 31] - 7.5) / 7.5
                        elif kind == "noise":
                            if int(phase + inc) != int(phase):
                                noise_val = rng.choice((-1.0, 1.0))
                            s = noise_val
                        else:
                            s = 1.0 if (phase % 1.0) < duty else -1.0
                        chunk.append(s * e * level * 8)
                        phase += inc * (8 if kind == "noise" else 1)
                for i, s in enumerate(chunk):
                    left[a + i] += s * gain_l
                    right[a + i] += s * gain_r
    return left, right, clock.seconds(end_tick)


def write_wav(path: Path, left: list[float], right: list[float], length: float, fade: float) -> None:
    n = min(len(left), int((length + fade) * RATE))
    peak = max(1e-9, max(max(abs(x) for x in left[:n]), max(abs(x) for x in right[:n])))
    scale = 0.9 / peak
    fade_from = int(length * RATE)
    out = bytearray()
    for i in range(n):
        g = scale
        if i >= fade_from and fade > 0:
            g *= max(0.0, 1 - (i - fade_from) / (fade * RATE))
        out += struct.pack("<hh", int(left[i] * g * 32767), int(right[i] * g * 32767))
    path.parent.mkdir(parents=True, exist_ok=True)
    with wave.open(str(path), "wb") as w:
        w.setnchannels(2)
        w.setsampwidth(2)
        w.setframerate(RATE)
        w.writeframes(bytes(out))


def cmd_render(args) -> int:
    rom = Rom(Path(args.rom))
    song = get_song(rom, args.song)
    left, right, length = render_song(rom, song, args.loops, args.fade)
    out = Path(args.out or f"song{args.song:03d}.wav")
    write_wav(out, left, right, length, args.fade)
    print(f"render: song {args.song}, {len(song.tracks)} tracks, {length:.1f} s + {args.fade} s fade -> {out}")
    return 0


# ---------------------------------------------------------------- DS units

# The DS side, as this game's ARM7 sound driver has it (src/arm7/sdk/snd/).
# The driver steps envelopes, LFOs and sequences once per sound frame: an
# alarm every 2728 ticks of the 33.513982 MHz / 64 OS timer
# (snd_start_interval_timer.c).
DS_FRAME = 2728 * 64 / 33513982
DS_CLOCK = 33513982 / 2                 # wave timer: DS_CLOCK / sample rate
# The volume curve: attenuation in 0.1 dB of each value 0-127 of volume,
# velocity, expression and sustain (gSndVolumeDecibels, 40 * log10(v / 127)):
# a square law. m4a volumes are linear.
DECIBEL_SQUARE = (
    -723, -722, -721, -651, -601, -562, -530, -503, -480, -460, -442, -425, -410, -396, -383, -371,
    -360, -349, -339, -330, -321, -313, -305, -297, -289, -282, -276, -269, -263, -257, -251, -245,
    -239, -234, -229, -224, -219, -214, -210, -205, -201, -196, -192, -188, -184, -180, -176, -173,
    -169, -165, -162, -158, -155, -152, -149, -145, -142, -139, -136, -133, -130, -127, -125, -122,
    -119, -116, -114, -111, -109, -106, -103, -101, -99, -96, -94, -91, -89, -87, -85, -82,
    -80, -78, -76, -74, -72, -70, -68, -66, -64, -62, -60, -58, -56, -54, -52, -50,
    -49, -47, -45, -43, -42, -40, -38, -36, -35, -33, -31, -30, -28, -27, -25, -23,
    -22, -20, -19, -17, -16, -14, -13, -11, -10, -8, -7, -6, -4, -3, -1, 0)
# Attack values 109 to 127 use this table (127 - value) instead of 255 - value (gSndAttackTable).
DS_ATTACK_TABLE = (0x00, 0x01, 0x05, 0x0e, 0x1a, 0x26, 0x33, 0x3f, 0x49, 0x54,
                   0x5c, 0x64, 0x6d, 0x74, 0x7b, 0x7f, 0x84, 0x89, 0x8f)
DS_SILENT = -92544                       # envelope start and end: -72.3 dB in 1/128 of 0.1 dB
CGB_STEPS = 15                           # a GB channel's envelope has 15 steps
DS_PSG_DUTY = (0, 1, 3, 5)               # GB duty 12.5, 25, 50, 75 % as DS PSG duty (n + 1) / 8
WAVE_RATE_AT_60 = 32 * 261.6256 / 2      # 32 steps a cycle, an octave below the key (MidiKeyToCgbFreq)
DS_MOD_RANGE_PITCH = 8                   # DS mod range with depth = MOD gives MOD / 16 semitones
SSEQ_TICKS = 2                           # SSEQ counts 48 ticks to the quarter note, m4a 24


def ds_volume(linear: float) -> int:
    """The DS value 0-127 whose square-law level is nearest a linear level 0-1."""
    if linear <= 0:
        return 0
    target = 200 * math.log10(min(1.0, linear))     # 0.1 dB
    return min(range(1, 128), key=lambda v: abs(DECIBEL_SQUARE[v] - target))


def ds_attack_coeff(v: int) -> int:
    return 255 - v if v < 109 else DS_ATTACK_TABLE[127 - v]


def ds_fall_coeff(v: int) -> int:
    """Decay and release: attenuation per sound frame in 1/128 of 0.1 dB
    (SndExChannel_CalcDecayRate)."""
    if v == 127:
        return 0xffff
    if v == 126:
        return 0x3c00
    if v < 50:
        return v * 2 + 1
    return 0x1e00 // (126 - v)


_ATTACK_CURVES: dict[int, list[float]] = {}


def ds_attack_curve(v: int) -> list[float]:
    """Linear level per sound frame of a DS attack, from silence to full."""
    if v not in _ATTACK_CURVES:
        k, att, out = ds_attack_coeff(v), DS_SILENT, []
        while att < 0 and len(out) < 4000:
            att = -((-att * k) >> 8)
            out.append(10 ** (att / 128 / 200))
        _ATTACK_CURVES[v] = out
    return _ATTACK_CURVES[v]


def ds_attack(seconds: float) -> int:
    """The DS attack nearest a linear rise from 0 to full in `seconds`: the
    one with the least squared difference of level over the rise."""
    if seconds <= 1 / 60:
        return 127
    frames = int(seconds / DS_FRAME) + 1
    best, best_err = 127, None
    for v in range(128):
        curve = ds_attack_curve(v)
        n = max(frames, len(curve)) + 1
        err = 0.0
        for f in range(n):
            ramp = min(1.0, (f + 1) / frames)
            level = curve[f] if f < len(curve) else 1.0
            err += (level - ramp) ** 2
        if best_err is None or err < best_err:
            best, best_err = v, err
    return best


def ds_fall(db_per_second: float) -> int:
    """The DS decay or release value nearest a fall of `db_per_second`."""
    if db_per_second == math.inf:
        return 127
    target = db_per_second * 1280 * DS_FRAME           # 1/128 of 0.1 dB per sound frame
    if target <= 0:
        return 0
    return min(range(127), key=lambda v: abs(math.log(ds_fall_coeff(v) / target)))


def ds_envelope(v: Voice) -> tuple[int, int, int, int]:
    """A voice's attack, decay, sustain and release in DS values (docs/m4a.md, "Envelopes").

    DirectSound: attack adds `attack` to a 0-255 level each frame (1/60 s);
    decay and release multiply the level by n/256 each frame, a fall of
    -20 log10(n/256) * 60 dB per second; sustain is a linear level n/255.
    GB channels: the level moves one of 15 steps every n frames (0: at once);
    sustain is n/15. The DS falls at a fixed rate in dB, so a GB decay or
    release, linear in level, maps to the DS rate that reaches the same
    level (sustain, or silence at -72.3 dB) in the same time.
    """
    if v.kind == "direct":
        attack = ds_attack(math.ceil(255 / max(v.attack, 1)) / 60)

        def fall(n: int) -> int:
            return 127 if n == 0 else ds_fall(-20 * math.log10(n / 256) * 60)
        return attack, fall(v.decay), ds_volume(v.sustain / 255), fall(v.release)
    a, d, s, r = v.attack & 7, v.decay & 7, v.sustain & 15, v.release & 7
    attack = ds_attack(CGB_STEPS * a / 60) if a else 127
    sustain = ds_volume(s / CGB_STEPS)
    if d == 0:
        decay = 127
    else:
        drop = 72.3 if s == 0 else -20 * math.log10(s / CGB_STEPS)
        decay = ds_fall(drop / ((CGB_STEPS - s) * d / 60)) if s < CGB_STEPS else 0
    release = 127 if r == 0 else ds_fall(72.3 / (CGB_STEPS * r / 60))
    return attack, decay, sustain, release


def ds_lfo_speed(lfos: int, bpm: int) -> int:
    """m4a's LFO steps `lfos`/256 of a cycle per tick (24 ticks a quarter);
    the DS's steps speed/512 of a cycle per sound frame."""
    return max(0, min(255, round(lfos * bpm * TICKS_PER_QUARTER / 60 / 256 * 512 * DS_FRAME)))


def ds_lfo_delay(ticks: int, bpm: int) -> int:
    """m4a counts the LFO delay in ticks, the DS in sound frames."""
    return min(0xffff, round(ticks * 60 / (bpm * TICKS_PER_QUARTER) / DS_FRAME))


def varlen(n: int) -> bytes:
    out = [n & 0x7f]
    n >>= 7
    while n:
        out.append(0x80 | n & 0x7f)
        n >>= 7
    return bytes(reversed(out))


# ---------------------------------------------------------------- import: waves and bank

@dataclass
class Report:
    warnings: list[str] = field(default_factory=list)

    def warn(self, msg: str) -> None:
        if msg not in self.warnings:
            self.warnings.append(msg)


def nitro_file(magic: bytes, data: bytes) -> bytes:
    """A NitroSystem file with one DATA block: header, block magic and size, data."""
    size = 0x10 + 8 + len(data)
    return magic + struct.pack("<HHIHH", 0xfeff, 0x100, size, 0x10, 1) + b"DATA" + struct.pack("<I", size - 0x10) + data


def swav(pcm: bytes, rate: float, loop_start: int | None) -> bytes:
    """One wave of a SWAR: 8-bit PCM. The DS loops on 4-byte words, so
    zero samples go before a loop start that is not on a word, and a loop
    whose length is not a whole number of words is repeated until it is."""
    if loop_start is None:
        body = pcm + bytes(-len(pcm) % 4)
        start_words, len_words, loop = 0, len(body) // 4, 0
    else:
        pad = -loop_start % 4
        part = pcm[loop_start:]
        repeat = 4 // math.gcd(len(part), 4) if part else 1
        body = bytes(pad) + pcm[:loop_start] + part * repeat
        start_words, len_words, loop = (pad + loop_start) // 4, len(part) * repeat // 4, 1
    timer = max(1, min(0xffff, round(DS_CLOCK / rate)))
    return struct.pack("<BBHHHI", 0, loop, min(0xffff, round(rate)), timer, start_words, len_words) + body


class Waves:
    """The SWAR being built: one wave per sample, shared by every voice that uses it."""

    def __init__(self, rom: Rom, fixed_rate: int) -> None:
        self.rom = rom
        self.fixed_rate = fixed_rate
        self.index: dict[tuple, int] = {}
        self.waves: list[bytes] = []

    def add(self, key: tuple, make) -> int:
        if key not in self.index:
            self.index[key] = len(self.waves)
            self.waves.append(make())
        return self.index[key]

    def sample(self, v: Voice) -> int:
        fixed = bool(v.type & TYPE_FIXED)

        def make() -> bytes:
            smp = read_sample(self.rom, v.wav)
            rate = self.fixed_rate if fixed else smp.rate
            return swav(smp.data, rate, smp.loop_start if smp.loop else None)
        return self.add(("sample", v.wav, fixed), make)

    def pattern(self, v: Voice) -> int:
        def make() -> bytes:
            steps = wave_pattern(self.rom, v.wav)
            pcm = bytes((n * 2 - 15) * 8 & 0xff for n in steps)
            return swav(pcm, WAVE_RATE_AT_60, 0)
        return self.add(("wave", v.wav), make)

    def swar(self) -> bytes:
        head = 0x3c + 4 * len(self.waves)
        offsets, body = [], bytearray()
        for w in self.waves:
            offsets.append(head + len(body))
            body += w + bytes(-len(w) % 4)
        data = bytes(32) + struct.pack(f"<I{len(self.waves)}I", len(self.waves), *offsets) + body
        return nitro_file(b"SWAR", data)


# SBNK instrument types.
INST_PCM, INST_PSG, INST_NOISE, INST_DRUMS, INST_SPLIT = 1, 2, 3, 16, 17


def instrument(waves: Waves, v: Voice, base: int, pan: int, report: Report) -> tuple[int, bytes]:
    """A simple SBNK instrument (type and 10 bytes) for a voice that plays a sound."""
    a, d, s, r = ds_envelope(v)
    kind = v.kind
    if kind == "direct":
        t, wave = INST_PCM, waves.sample(v)
    elif kind in ("square1", "square2"):
        t, wave = INST_PSG, DS_PSG_DUTY[v.wav & 3]
        if kind == "square1" and v.pan_sweep & 0x70 and not v.pan_sweep & 0x80:
            report.warn(f"voice at {v.addr:#x}: square 1 sweep {v.pan_sweep:#x} dropped "
                        "(the DS has none per instrument)")
    elif kind == "wave":
        t, wave = INST_PCM, waves.pattern(v)
    else:
        t, wave = INST_NOISE, 0
        report.warn(f"voice at {v.addr:#x}: GB noise as DS noise; its pitch and 7-bit mode map loosely")
    return t, struct.pack("<HHBBBBBB", wave, 0, max(0, min(127, base)), a, d, s, r, max(0, min(127, pan)))


def leaf(rom: Rom, waves: Waves, v: Voice, key: int, drums: bool, report: Report) -> tuple[int, bytes]:
    """The instrument for one key of a key split or drum kit.

    m4a plays a key split's voice at the track's key, and a drum's voice at
    the drum's own key with the drum's pan; the DS plays the note at the
    track's key relative to the base note, so a drum's base note becomes
    key - drum key + 60. A fixed-pitch voice plays at the mixing
    rate: its wave gets that rate and its base note equals the key.
    """
    if v.type & (TYPE_SPLIT | TYPE_DRUMS):
        report.warn(f"voice at {v.addr:#x}: a key split or drum kit inside another; key {key} left silent")
        return 0, bytes(10)
    pan = 64
    base = 60
    if drums:
        base = key - v.key + 60
        if v.pan_sweep & 0x80:
            pan = 64 + v.pan_sweep - 0xc0
    if v.type & TYPE_FIXED and v.kind == "direct":
        base = key
    return instrument(waves, v, base, pan, report)


def bank_entry(rom: Rom, waves: Waves, v: Voice, keys: set[int], report: Report) -> tuple[int, bytes]:
    """One SBNK instrument for a voicegroup entry, given the keys the song plays on it."""
    if v.type & (TYPE_SPLIT | TYPE_DRUMS) or (v.type & TYPE_FIXED and v.kind == "direct"):
        drums = bool(v.type & TYPE_DRUMS)
        lo, hi = min(keys), max(keys)
        subs = {k: sub_voice(rom, v, k) if v.type & (TYPE_SPLIT | TYPE_DRUMS) else v for k in range(lo, hi + 1)}
        if v.type & TYPE_SPLIT:
            # Runs of keys on one voice; keys below and above the song's range join the end runs.
            runs: list[list] = []
            for k in range(lo, hi + 1):
                if runs and subs[k].addr == runs[-1][1].addr:
                    runs[-1][0] = k
                else:
                    runs.append([k, subs[k]])
            if len(runs) <= 8:
                runs[-1][0] = 127
                head = bytes(r[0] for r in runs) + bytes(8 - len(runs))
                body = b"".join(struct.pack("<H", t) + data for t, data in
                                (leaf(rom, waves, r[1], r[0], False, report) for r in runs))
                return INST_SPLIT, head + body
        # A drum set: one instrument per key from lo to hi.
        body = bytes((lo, hi))
        for k in range(lo, hi + 1):
            t, data = leaf(rom, waves, subs[k], k, drums, report)
            body += struct.pack("<H", t) + data
        return INST_DRUMS, body
    return instrument(waves, v, 60, 64, report)


def build_sbnk(entries: list[tuple[int, bytes] | None]) -> bytes:
    head = 0x3c + 4 * len(entries)
    records, body = bytearray(), bytearray()
    for e in entries:
        if e is None:
            records += bytes(4)
            continue
        t, data = e
        records += struct.pack("<BHB", t, head + len(body), 0)
        body += data
    body += bytes(-(head + len(body)) % 4)
    data = bytes(32) + struct.pack("<I", len(entries)) + records + body
    return nitro_file(b"SBNK", data)


# ---------------------------------------------------------------- import: sequence

def voice_at(rom: Rom, song: Song, number: int) -> Voice:
    return read_voice(rom, song.voicegroup + 12 * number)


def sounding_voice(rom: Rom, v: Voice, key: int) -> Voice:
    return sub_voice(rom, v, key) if v.type & (TYPE_SPLIT | TYPE_DRUMS) else v


def tie_mode_ties(runs: list[TrackRun], report: Report) -> tuple[set[int], set[int]]:
    """TIEs to play in the DS's tie mode, and the EOTs that end them.

    A TIE becomes a note of fixed length, the time to its EOT. When one TIE
    (one place in the ROM, such as in a pattern) lasts different times on
    different passes, no one length fits: it is played in tie mode (0xc8 1,
    a note that lasts until 0xc8 0 at the EOT). The DS's tie mode releases
    the track's other notes when it starts and plays the track's next notes
    on the tied note's channel, so it is used only when the track plays no
    other note while the tie lasts.
    """
    lengths: dict[int, set[int]] = {}
    for run in runs:
        for p in run.played:
            if p.event.kind == "note" and p.event.args[2] == 0:
                lengths.setdefault(p.event.pc, set()).add(p.length)
    mixed = {pc for pc, ls in lengths.items() if len(ls) > 1}
    tie_mode, ends, unsafe = set(), set(), set()
    for run in runs:
        notes = [p for p in run.played if p.event.kind == "note"]
        for p in notes:
            if p.event.pc not in mixed:
                continue
            start, end = p.time, p.time + p.length
            for q in notes:
                if q is not p and (start < q.time < end or q.time < start < q.time + q.length
                                   or (q.time == start and q.event.pc != p.event.pc)):
                    unsafe.add(p.event.pc)
            if p.ended_by is not None:
                ends.add(p.ended_by)
    for pc in sorted(mixed):
        if pc in unsafe:
            report.warn(f"{pc:#x}: a TIE lasts {sorted(lengths[pc])} ticks on different passes and overlaps "
                        "other notes; the first length is used")
        else:
            tie_mode.add(pc)
    if unsafe:
        ends = {pc for run in runs for p in run.played
                if p.event.pc in tie_mode and p.ended_by is not None for pc in [p.ended_by]}
    return tie_mode, ends


@dataclass
class Label:
    """A jump, call or track start to an m4a address, filled in at layout."""
    addr: int


def convert_tracks(rom: Rom, song: Song, report: Report) -> tuple[bytes, dict[int, set[int]]]:
    """The song's tracks as SSEQ events, and the keys played on each voice.

    Each track is run as the driver runs it (two times through the loop),
    and every command becomes DS commands at the place it has in the ROM,
    so PATT/PEND become call/return and GOTO a jump. A command reached more
    than once must give the same DS commands each time (a TIE's length, a
    bend that depends on BENDR); when it does not, the first is used and a
    warning says so.
    """
    runs = [run_track(rom, t, 2) for t in song.tracks]
    for r in runs:
        for w in r.warnings:
            report.warn(w)
    tempos = tempo_map(runs)
    bpm = tempos[1][1] if len(tempos) > 1 and tempos[1][0] == 0 else tempos[0][1]
    if len({b for _, b in tempos[1:]}) > 1:
        report.warn("the tempo changes; LFO speeds and delays use the first tempo")
    tie_mode, tie_ends = tie_mode_ties(runs, report)
    keys: dict[int, set[int]] = {}
    visits: dict[int, list[tuple]] = {}
    sizes: dict[int, Event] = {}
    for run in runs:
        bend = tune = keysh = voice = 0
        bendr = range_ds = 2
        for p in run.played:
            ev = p.event
            k = ev.kind
            code: list = []
            if k == "wait":
                if ev.args[0]:
                    code.append(b"\x80" + varlen(ev.args[0] * SSEQ_TICKS))
            elif k == "note":
                key, velocity = ev.args[0], ev.args[1]
                keys.setdefault(voice, set()).add(key)
                v = voice_at(rom, song, voice)
                if keysh and v.type & (TYPE_SPLIT | TYPE_DRUMS):
                    report.warn(f"{ev.pc:#x}: KEYSH {keysh} on a key split or drum kit; "
                                "the DS picks the entry by the shifted key, m4a by the key")
                gain = CGB_LEVEL if sounding_voice(rom, v, key).kind != "direct" else 1.0
                length = max(1, p.length) * SSEQ_TICKS
                if ev.pc in tie_mode:
                    code.append(b"\xc8\x01")
                    length = 0
                code.append(bytes((key, ds_volume(velocity / 127 * gain))) + varlen(length))
            elif k == "eot":
                if ev.pc in tie_ends:
                    code.append(b"\xc8\x00")         # ends a TIE played in tie mode
                # otherwise the TIE's note has its length
            elif k == "tempo":
                code.append(b"\xe1" + struct.pack("<H", ev.args[0]))
            elif k == "keysh":
                keysh = ev.args[0]
                code.append(b"\xc3" + struct.pack("<b", keysh))
            elif k == "voice":
                voice = ev.args[0]
                code.append(b"\x81" + varlen(voice))
            elif k == "vol":
                code.append(bytes((0xc1, ds_volume(ev.args[0] / 127))))
            elif k == "pan":
                code.append(bytes((0xc0, max(0, min(127, ev.args[0] + 64)))))
            elif k in ("bend", "bendr", "tune"):
                if k == "bend":
                    bend = ev.args[0]
                elif k == "tune":
                    tune = ev.args[0]
                else:
                    bendr = ev.args[0]
                # m4a: (bend * bendr + tune) / 64 semitones; DS: bend * range / 128.
                # The DS range is BENDR, or wider when TUNE takes the bend past 127.
                total = bend * bendr + tune
                want_range = max(bendr if bendr else 1, math.ceil(2 * abs(total) / 127))
                if want_range != range_ds:
                    range_ds = want_range
                    code.append(bytes((0xc5, range_ds)))
                ds_bend = round(2 * total / range_ds)
                if not -128 <= ds_bend <= 127:
                    report.warn(f"{ev.pc:#x}: bend {ds_bend} clamped to the DS range")
                code.append(b"\xc4" + struct.pack("<b", max(-128, min(127, ds_bend))))
            elif k == "lfos":
                code.append(bytes((0xcb, ds_lfo_speed(ev.args[0], bpm))))
            elif k == "lfodl":
                code.append(b"\xe0" + struct.pack("<H", ds_lfo_delay(ev.args[0], bpm)))
            elif k == "mod":
                code.append(bytes((0xca, min(127, ev.args[0]))))
            elif k == "modt":
                t = ev.args[0] if ev.args[0] <= 2 else 0
                code.append(bytes((0xcc, t, 0xcd, DS_MOD_RANGE_PITCH if t == 0 else 1)))
            elif k == "prio":
                pass                                  # channel priorities stay the DS defaults
            elif k == "goto":
                code += [b"\x94", Label(ev.args[0])]
            elif k == "patt":
                code += [b"\x95", Label(ev.args[0])]
            elif k == "pend":
                code.append(b"\xfd")
            elif k == "fine":
                code.append(b"\xff")
            elif k == "rept":
                if ev.args[0] == 0:
                    code += [b"\x94", Label(ev.args[1])]
                else:
                    report.warn(f"{ev.pc:#x}: REPT {ev.args[0]} not converted; it plays once")
            else:
                report.warn(f"{ev.pc:#x}: {k.upper()} {ev.args} not converted")
            visits.setdefault(ev.pc, []).append(tuple(code))
            sizes[ev.pc] = ev
    chosen: dict[int, tuple] = {}
    for pc, codes in visits.items():
        chosen[pc] = codes[0]
        if any(c != codes[0] for c in codes):
            report.warn(f"{pc:#x}: {sizes[pc].kind} gives different DS commands on different passes; the first is used")

    # Layout: allocate tracks, open tracks 1 and up, one stub per track
    # (notes do not wait, the m4a LFO defaults), then the commands in ROM order.
    out = bytearray()
    fixups: list[tuple[int, int]] = []
    labels: dict[int, int] = {}
    n = len(song.tracks)
    if n > 16:
        sys.exit(f"error: song {song.index} has {n} tracks; an SSEQ has at most 16")
    out += b"\xfe" + struct.pack("<H", (1 << n) - 1)
    opens = []
    for i in range(1, n):
        out += bytes((0x93, i))
        opens.append(len(out))
        out += bytes(3)
    stubs = []
    for t in song.tracks:
        stubs.append(len(out))
        out += bytes((0xc7, 0, 0xcb, ds_lfo_speed(22, bpm), 0xcd, DS_MOD_RANGE_PITCH, 0x94))
        fixups.append((len(out), t))
        out += bytes(3)
    for at, stub in zip(opens, stubs[1:]):
        out[at:at + 3] = stub.to_bytes(3, "little")
    for pc in sorted(chosen):
        labels[pc] = len(out)
        for part in chosen[pc]:
            if isinstance(part, Label):
                fixups.append((len(out), part.addr))
                out += bytes(3)
            else:
                out += part
        ev = sizes[pc]
        if ev.kind not in ("goto", "fine", "pend") and not (ev.kind == "rept" and ev.args[0] == 0) \
                and pc + ev.size not in chosen:
            out += b"\xff"                            # the run stopped here (cut track)
    for at, addr in fixups:
        if addr not in labels:
            sys.exit(f"error: jump to {addr:#x}, which no track reaches")
        out[at:at + 3] = labels[addr].to_bytes(3, "little")
    out += bytes(-len(out) % 4)
    size = 0x1c + len(out)
    sseq = b"SSEQ" + struct.pack("<HHIHH", 0xfeff, 0x100, size, 0x10, 1)
    sseq += b"DATA" + struct.pack("<II", size - 0x10, 0x1c) + out
    return sseq, keys


def convert(rom: Rom, song: Song, report: Report) -> tuple[bytes, bytes, bytes]:
    """The song as SSEQ, SBNK and SWAR files."""
    rate, found = mix_rate(rom)
    sseq, keys = convert_tracks(rom, song, report)
    waves = Waves(rom, rate)
    entries: list[tuple[int, bytes] | None] = [None] * (max(keys) + 1 if keys else 0)
    for number in sorted(keys):
        v = voice_at(rom, song, number)
        if v.type & TYPE_FIXED and v.kind == "direct" and not found:
            report.warn(f"mixing rate not found in the ROM; fixed-pitch voices use {rate} Hz")
        entries[number] = bank_entry(rom, waves, v, keys[number], report)
    return sseq, build_sbnk(entries), waves.swar()


# ---------------------------------------------------------------- import: sound.yaml

def upsert(lines: list[str], kind: str, name: str, row: dict) -> int:
    """Put an entry named `name` in sound.yaml's `kind` list: in place if
    one has the name, at the end otherwise. Returns its id."""
    import sound
    head = next((i for i, line in enumerate(lines) if line.split(":")[0] == kind), None)
    if head is None:
        sys.exit(f"error: sound.yaml has no {kind} list")
    if lines[head].strip() == f"{kind}: []":
        lines[head] = f"{kind}:"
    end = head + 1
    while end < len(lines) and lines[end].startswith("- "):
        end += 1
    for i in range(head + 1, end):
        entry = sound.yaml.safe_load(lines[i][2:])
        if entry.get("name") == name:
            lines[i] = "- " + sound.flow({"id": entry["id"], "name": name} | row)
            return entry["id"]
    new_id = end - head - 1
    lines.insert(end, "- " + sound.flow({"id": new_id, "name": name} | row))
    return new_id


def entry_named(doc: dict, kind: str, name: str) -> dict | None:
    return next((e for e in doc.get(kind) or [] if e.get("name") == name), None)


def find_seq(doc: dict, like: str) -> dict:
    rows = doc.get("seq") or []
    row = rows[int(like)] if like.isdigit() and int(like) < len(rows) else entry_named(doc, "seq", like)
    if row is None:
        sys.exit(f"error: no sequence {like} in sound.yaml")
    return row


def cmd_import(args) -> int:
    import sound
    rom = Rom(Path(args.rom))
    song = get_song(rom, args.song)
    if not re.fullmatch(r"[A-Z0-9_]+", args.name):
        sys.exit("error: --name takes capitals, digits and _ (a SYMB name, such as BGM_MY_SONG)")
    report = Report()
    sseq, sbnk, swar = convert(rom, song, report)

    src = ROOT / args.sound
    manifest = src / sound.MANIFEST
    if not manifest.exists():
        sys.exit(f"error: {manifest} not found; run make sound")
    text = manifest.read_text()
    doc = sound.yaml.safe_load(text)
    like = find_seq(doc, args.like)
    names = {"seq": args.name, "bank": f"BANK_{args.name}", "wavearc": f"WAVE_{args.name}"}
    # File ids: an entry already imported keeps its own; new ones go after the last.
    used = [e["fileId"] for k in sound.FILE_KINDS for e in doc.get(k) or [] if e and "fileId" in e]
    used += [e["fileId"] for e in doc.get("unusedFiles") or []]
    next_file = max(used) + 1
    file_ids, ids = {}, {}
    for kind in ("seq", "bank", "wavearc"):
        old = entry_named(doc, kind, names[kind])
        if old:
            file_ids[kind], ids[kind] = old["fileId"], old["id"]
        else:
            file_ids[kind], ids[kind] = next_file, len(doc.get(kind) or [])
            next_file += 1
    paths = {kind: sound.file_name(kind, ids[kind], names[kind]) for kind in names}
    lines = text.rstrip("\n").split("\n")
    upsert(lines, "wavearc", names["wavearc"], {"file": paths["wavearc"], "fileId": file_ids["wavearc"]})
    upsert(lines, "bank", names["bank"], {"file": paths["bank"], "fileId": file_ids["bank"],
                                         "waveArc": [ids["wavearc"]]})
    upsert(lines, "seq", names["seq"], {"file": paths["seq"], "fileId": file_ids["seq"], "bankNo": ids["bank"],
                                       "volume": like["volume"] if args.volume is None else args.volume,
                                       "channelPrio": like["channelPrio"],
                                       "playerPrio": like["playerPrio"], "playerNo": like["playerNo"]})
    for kind, data in (("seq", sseq), ("bank", sbnk), ("wavearc", swar)):
        sound.write_if_changed(src / paths[kind], data)
    sound.write_if_changed(manifest, "\n".join(lines) + "\n")

    total = len(sseq) + len(sbnk) + len(swar)
    players = doc.get("player") or []
    heap = players[like["playerNo"]]["heapSize"] if like["playerNo"] < len(players) else None
    print(f"import: song {song.index} as seq {ids['seq']} {names['seq']}, bank {ids['bank']}, wave archive "
          f"{ids['wavearc']} (file ids {file_ids['seq']}, {file_ids['bank']}, {file_ids['wavearc']})")
    print(f"  sizes: sequence {len(sseq)}, bank {len(sbnk)}, wave archive {len(swar)}, together {total} bytes"
          + (f" of player {like['playerNo']}'s {heap}-byte heap" if heap else ""))
    for w in report.warnings:
        print(f"  warning: {w}")
    if heap and total > heap:
        sys.exit(f"error: the song needs {total} bytes; player {like['playerNo']}'s heap holds {heap}")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = parser.add_subparsers(required=True)
    p = sub.add_parser("list", help="every song of the ROM's song table")
    p.add_argument("rom")
    p.add_argument("--all", action="store_true", help="also the empty entries")
    p.set_defaults(func=cmd_list)
    p = sub.add_parser("render", help="render one song to a WAV file")
    p.add_argument("rom")
    p.add_argument("song", type=int)
    p.add_argument("--out", help="WAV file (default songNNN.wav)")
    p.add_argument("--loops", type=int, default=2, help="times through the loop (default 2)")
    p.add_argument("--fade", type=float, default=8, help="seconds of fade after the last loop (default 8)")
    p.set_defaults(func=cmd_render)
    p = sub.add_parser("import", help="convert one song into the unpacked sound archive")
    p.add_argument("rom")
    p.add_argument("song", type=int)
    p.add_argument("--name", required=True, help="the sequence's SYMB name, such as BGM_MY_SONG")
    p.add_argument("--like", default="0",
                   help="sequence (id or name) whose volume, priorities and player the new one takes (default 0)")
    p.add_argument("--volume", type=int, choices=range(128), metavar="0-127",
                   help="the sequence's volume, in place of --like's")
    p.add_argument("--sound", default="extracted/AWRE/assets/sound", help="the unpacked archive (make sound)")
    p.set_defaults(func=cmd_import)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())

# melonDS and the GDB remote protocol

- **Project:** melonDS, a Nintendo DS emulator.
- **Authors:** Arisotura and the melonDS contributors.
- **URL:** https://github.com/melonDS-emu/melonDS
- **Version we use:** 1.1, built from source by the user with its GDB stub. Not in `tools.lock`: `tools/melonds_check.py` finds it beside the repo's main checkout or through `MELONDS` (`docs/workflow.md`).
- **License:** GPL-3.0.

## What we take

Facts only, read from the melonDS source (`src/debug/`, `src/ARM.cpp`, `src/ARMInterpreter.cpp`, `src/frontend/qt_sdl/Config.cpp`, `main.cpp`):

- How its GDB stub talks: the client sends `+` first after connecting, connecting stops the target, stop replies are `S05` with no registers, an undefined instruction stops with `S04` before the exception is taken, `m` reads at most 576 bytes, `D` and `k` both detach. The order of the registers in a `g` reply (`GdbArch.h`).
- That the stub sends its ack and its reply as two writes without `TCP_NODELAY`, which is why `tools/gdb_rsp.py` sets quick acks.
- Where the ARM9 exception entries go (`ExceptionBase` + 0x04, 0x0c, 0x10, at 0xffff0000 on the DS) and what lands in `lr_und` and `lr_abt`.
- The names of the config keys (`Instance0.Gdb.ARM9.Port`, `BreakOnStartup`, `Instance0.Audio.Volume`, `SaveFilePath`, `LimitFPS`) and that the config directory is `$XDG_CONFIG_HOME/melonDS`.

The packet format (`$data#checksum`, `+` and `-` acks, 0x03 to stop, `}` escapes, run-length encoding, the `?`, `g`, `m`, `P`, `Z0`, `z0`, `c`, `D`, `k` packets) comes from the GDB manual, appendix "GDB Remote Serial Protocol" (https://sourceware.org/gdb/current/onlinedocs/gdb.html/Remote-Protocol.html), GNU Free Documentation License.

## What we do not take

No code from either. `tools/gdb_rsp.py` and `tools/melonds_check.py` are our own; melonDS runs unchanged as a separate program.

# melonDS, the GDB remote protocol and RFB

- **Project:** melonDS, a Nintendo DS emulator.
- **Authors:** Arisotura and the melonDS contributors.
- **URL:** https://github.com/melonDS-emu/melonDS
- **Version we use:** 1.1, built from source by the user with its GDB stub. Not in `tools.lock`: `tools/melonds.py` finds it beside the repo's main checkout or through `MELONDS` (`docs/workflow.md`).
- **License:** GPL-3.0.

## What we take

Facts only, read from the melonDS source (`src/debug/`, `src/ARM.cpp`, `src/ARMInterpreter.cpp`, `src/frontend/qt_sdl/Config.cpp`, `main.cpp`):

- How its GDB stub talks: the client sends `+` first after connecting, connecting stops the target, stop replies are `S05` with no registers, an undefined instruction stops with `S04` before the exception is taken, `m` reads at most 576 bytes, `D` and `k` both detach. The order of the registers in a `g` reply (`GdbArch.h`).
- That the stub sends its ack and its reply as two writes without `TCP_NODELAY`, which is why `tools/gdb_rsp.py` sets quick acks.
- Where the ARM9 exception entries go (`ExceptionBase` + 0x04, 0x0c, 0x10, at 0xffff0000 on the DS) and what lands in `lr_und` and `lr_abt`.
- The names of the config keys (`Instance0.Gdb.ARM9.Port`, `BreakOnStartup`, `Instance0.Audio.Volume`, `SaveFilePath`, `LimitFPS`) and that the config directory is `$XDG_CONFIG_HOME/melonDS`.

Seen in runs of melonDS 1.1, and used by `tools/emu.py`:

- melonDS polls the GDB stub once per emulated frame, so a stop request (0x03) is answered at the next frame. A memory read (`m`) while the target runs does not stop the CPU.
- Watchpoints (`Z2`, `Z3`, `Z4`) and breakpoint conditions are accepted but never fire. The tools use plain `Z0` breakpoints only.
- melonDS writes its config file back on exit, so the tools write each run's config again before every launch, in a directory of the tool's own.
- melonDS logs every GDB packet to its output, about 2.4 MB a minute when stopping every frame; the tools send that output to a file or to `/dev/null`.

melonDS's frontend is a Qt program, so `QT_QPA_PLATFORM` picks where it draws. With `QT_QPA_PLATFORM=vnc:size=WxH:port=N` (Qt's VNC platform plugin) its window is drawn to a virtual screen served over VNC, which `tools/emu.py` reads for screenshots, with no display. The plugin has no option for the address it listens on and listens on all network interfaces, as the GDB stub does, while a run lasts. The VNC client in `tools/emu.py` speaks the Remote Framebuffer protocol: the version handshake (3.3 or 3.8), security type None, ClientInit and ServerInit, `SetPixelFormat`, `SetEncodings` with raw encoding only, and `FramebufferUpdateRequest` and `FramebufferUpdate`, from RFC 6143, "The Remote Framebuffer Protocol" (https://www.rfc-editor.org/rfc/rfc6143), under the IETF Trust Legal Provisions. We take only those facts.

The packet format (`$data#checksum`, `+` and `-` acks, 0x03 to stop, `}` escapes, run-length encoding, the `?`, `g`, `m`, `P`, `Z0`, `z0`, `c`, `D`, `k` packets) comes from the GDB manual, appendix "GDB Remote Serial Protocol" (https://sourceware.org/gdb/current/onlinedocs/gdb.html/Remote-Protocol.html), GNU Free Documentation License.

## What we do not take

No code from any of them. `tools/gdb_rsp.py`, `tools/melonds.py`, `tools/melonds_check.py` and `tools/emu.py` are our own; melonDS runs unchanged as a separate program.

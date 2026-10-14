#!/usr/bin/env python3
"""Check the GDB remote protocol client and the parts of melonds_check.py that need no emulator.

    python3 tools/tests/test_gdb_rsp.py

Packet encoding and decoding, the `g` and stop replies, and a session with a
fake stub that behaves like melonDS's (it waits for `+` after the connect,
acks each packet, and sends a stop reply only when the target stops). Then
the key script, the overlay table end and the ELF symbol reader. It never
starts melonDS: `make emu-check` boots the ROMs.
"""

import socket
import struct
import sys
import tempfile
import threading
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(TOOLS))

import gdb_rsp  # noqa: E402
import melonds_check  # noqa: E402

errors = []


def expect(what, got, want):
    if got != want:
        errors.append(f"{what}: want {want!r}, got {got!r}")


# Packets

expect("encode", gdb_rsp.encode("m2000000,4"), b"$m2000000,4#" + b"%02x" % (sum(b"m2000000,4") & 0xFF))
expect("encode escapes", gdb_rsp.encode(b"a#b"), b"$a}\x03b#" + b"%02x" % ((ord("a") + 0x7D + 3 + ord("b")) & 0xFF))
expect("unescape", gdb_rsp.unescape(b"a}\x03b"), b"a#b")
expect("run length", gdb_rsp.unescape(b"0* "), b"0000")  # ' ' is 32: three more
payload, rest, ok = gdb_rsp.decode(b"+$OK#9a+$S05")
expect("decode", (payload, rest, ok), (b"OK", b"+$S05", True))
expect("decode partial", gdb_rsp.decode(b"$S05#")[0], None)
expect("decode bad checksum", gdb_rsp.decode(b"$OK#00")[2], False)
expect("stop S05", gdb_rsp.parse_stop(b"S05"), 5)
expect("stop T04", gdb_rsp.parse_stop(b"T04hwbreak:;"), 4)
expect("not a stop", gdb_rsp.parse_stop(b"0123abcd"), None)
regs = gdb_rsp.parse_registers(b"".join(struct.pack("<I", i * 0x01010101).hex().encode()
                                        for i in range(len(gdb_rsp.REGISTERS))))
expect("register r1", regs["r1"], 0x01010101)
expect("register pc", regs["pc"], 15 * 0x01010101)
expect("register spsr_und", regs["spsr_und"], (len(gdb_rsp.REGISTERS) - 1) * 0x01010101 & 0xFFFFFFFF)


# A fake stub

class FakeStub(threading.Thread):
    """Answers like melonDS: memory reads of 0x11 bytes, one breakpoint stop after `c`."""

    def __init__(self, sock):
        super().__init__(daemon=True)
        self.sock = sock
        self.log = []

    def reply(self, payload):
        self.sock.sendall(gdb_rsp.encode(payload))
        assert self.sock.recv(1) == b"+"

    def run(self):
        assert self.sock.recv(1) == b"+"
        self.sock.sendall(b"+")
        buf = b""
        while True:
            data = self.sock.recv(4096)
            if not data:
                return
            buf += data
            while True:
                if buf[:1] == b"\x03":
                    buf = buf[1:]
                    self.log.append("interrupt")
                    self.reply("S02")
                    continue
                payload, rest, ok = gdb_rsp.decode(buf)
                if payload is None:
                    break
                buf = rest
                if not ok:
                    self.sock.sendall(b"-")
                    continue
                cmd = payload.decode()
                self.log.append(cmd)
                if cmd == "k":
                    return  # the client closes without waiting for the ack
                self.sock.sendall(b"+")
                if cmd == "?":
                    self.reply("S02")
                elif cmd.startswith("m"):
                    _addr, length = cmd[1:].split(",")
                    self.reply("11" * int(length, 16))
                elif cmd.startswith(("Z0", "z0", "P")):
                    self.reply("OK")
                elif cmd == "c":
                    # The target runs; a breakpoint stops it later. A stale stop
                    # reply follows, as when an interrupt crosses a breakpoint.
                    self.reply("S05")
                elif cmd == "g":
                    self.reply("S02")  # stale
                    self.reply("00000000" * len(gdb_rsp.REGISTERS))


a, b = socket.socketpair()
stub = FakeStub(b)
stub.start()
client = gdb_rsp.GdbClient(a)
a.sendall(b"+")
client._expect_ack()
expect("halt", client.halt_reason(), 2)
client.set_breakpoint(0xFFFF0010)
expect("read across packets", client.read(0x02000000, 600), b"\x11" * 600)
client.resume()
expect("stop after c", client.wait_stop(2), 5)
expect("registers skip a stale stop", client.registers()["pc"], 0)
client.write_register("r1", 8)
client.kill()
stub.join(2)
expect("packets", [c for c in stub.log if not c.startswith("m")],
       ["?", "Z0,ffff0010,4", "c", "g", "P1=08000000", "k"])
expect("read split", [c for c in stub.log if c.startswith("m")], ["m02000000,00000200", "m02000200,00000058"])


# melonds_check parts

expect("presses", melonds_check.parse_presses("2760:start,0x10:a+b"), [(2760, 8), (16, 3)])

with tempfile.TemporaryDirectory() as tmp:
    rom = bytearray(0x200)
    struct.pack_into("<II", rom, 0x50, 0x100, 64)
    struct.pack_into("<IIII", rom, 0x100, 0, 0x022AD560, 0x1000, 0x20)
    struct.pack_into("<IIII", rom, 0x120, 1, 0x02350560, 0x2A000, 0x40)
    (Path(tmp) / "a.nds").write_bytes(rom)
    expect("overlays end", melonds_check.overlays_end(Path(tmp) / "a.nds"), 0x0237A5A0)

    # An ELF with one section header table: null, .symtab, .strtab.
    strtab = b"\0Foo\0Bar\0"
    symtab = (b"\0" * 16 + struct.pack("<IIIBBH", 1, 0x02000801, 4, 0x12, 0, 1)
              + struct.pack("<IIIBBH", 5, 0x02100000, 4, 0x11, 0, 1))
    body = symtab + strtab
    shoff = 0x34 + len(body)
    header = bytearray(0x34)
    header[:6] = b"\x7fELF\x01\x01"
    struct.pack_into("<I", header, 0x20, shoff)
    struct.pack_into("<HH", header, 0x2E, 40, 3)
    sections = (b"\0" * 40
                + struct.pack("<IIIIIIIIII", 0, 2, 0, 0, 0x34, len(symtab), 2, 0, 4, 16)
                + struct.pack("<IIIIIIIIII", 0, 3, 0, 0, 0x34 + len(symtab), len(strtab), 0, 0, 1, 0))
    (Path(tmp) / "a.elf").write_bytes(bytes(header) + body + sections)
    expect("elf symbols", melonds_check.elf_symbols(Path(tmp) / "a.elf"), {"Foo": 0x02000800})


if errors:
    sys.exit("test_gdb_rsp: " + "\n  ".join(errors))
print("test_gdb_rsp: ok")

"""A small client for the GDB remote serial protocol, as melonDS's GDB stub speaks it.

Packets are `$data#cc`, where cc is the sum of the data bytes modulo 256 in
two hex digits. Each side answers a packet with `+` (received) or `-` (send
again). A lone 0x03 byte asks a running target to stop.

melonDS's stub (src/debug/ in its source) has some habits a client must
follow:

  - After connecting, the client sends `+` first; the stub waits up to a
    second for it, then answers `+`.
  - Connecting stops the target. Each stop is announced with a short reply,
    `S05` for a breakpoint or a stop request, `S04` for an undefined
    instruction. The reply holds no registers: read them with `g`.
  - `?` also stops the target.
  - `m` reads at most 576 bytes per packet.
  - `D` and `k` both detach; the emulator keeps running.

Facts about melonDS's stub and the protocol: docs/third-party/melonds.md.
Used by tools/melonds_check.py. Tested without an emulator by
tools/tests/test_gdb_rsp.py.
"""

import socket
import time

# Registers in the order of melonDS's `g` reply (src/debug/GdbArch.h).
REGISTERS = (
    "r0 r1 r2 r3 r4 r5 r6 r7 r8 r9 r10 r11 r12 sp lr pc cpsr "
    "sp_usr lr_usr r8_fiq r9_fiq r10_fiq r11_fiq r12_fiq sp_fiq lr_fiq "
    "sp_irq lr_irq sp_svc lr_svc sp_abt lr_abt sp_und lr_und "
    "spsr_fiq spsr_irq spsr_svc spsr_abt spsr_und").split()

MAX_READ = 512  # bytes per `m` packet; melonDS's buffer takes 576


class ProtocolError(Exception):
    pass


def checksum(data: bytes) -> int:
    return sum(data) & 0xFF


def escape(data: bytes) -> bytes:
    """Escapes the bytes that may not appear raw in a packet: `#`, `$`, `}` and `*`."""
    out = bytearray()
    for b in data:
        if b in b"#$}*":
            out += bytes((0x7D, b ^ 0x20))
        else:
            out.append(b)
    return bytes(out)


def unescape(data: bytes) -> bytes:
    """Undoes `escape` and expands run-length encoding (`x*n`: x repeated n - 29 more times)."""
    out = bytearray()
    i = 0
    while i < len(data):
        b = data[i]
        if b == 0x7D:
            out.append(data[i + 1] ^ 0x20)
            i += 2
        elif b == 0x2A and out:
            out += bytes((out[-1],)) * (data[i + 1] - 29)
            i += 2
        else:
            out.append(b)
            i += 1
    return bytes(out)


def encode(payload: str | bytes) -> bytes:
    """One packet, `$payload#cc`."""
    data = escape(payload.encode() if isinstance(payload, str) else payload)
    return b"$" + data + b"#%02x" % checksum(data)


def decode(buffer: bytes) -> tuple[bytes | None, bytes, bool]:
    """Takes the first packet out of `buffer`.

    Returns (payload, rest, checksum_ok). The payload is None when the buffer
    holds no complete packet yet; acks (`+`, `-`) before the packet are
    dropped.
    """
    start = buffer.find(b"$")
    if start < 0:
        return None, buffer, True
    end = buffer.find(b"#", start)
    if end < 0 or len(buffer) < end + 3:
        return None, buffer[start:], True
    data = buffer[start + 1:end]
    try:
        ok = int(buffer[end + 1:end + 3], 16) == checksum(data)
    except ValueError:
        ok = False
    return unescape(data), buffer[end + 3:], ok


def parse_registers(reply: bytes) -> dict[str, int]:
    """Decodes a `g` reply: 8 hex digits per register, little-endian bytes."""
    regs = {}
    for i, name in enumerate(REGISTERS):
        chunk = reply[i * 8:i * 8 + 8]
        if len(chunk) < 8:
            break
        regs[name] = int.from_bytes(bytes.fromhex(chunk.decode()), "little")
    return regs


def parse_stop(reply: bytes) -> int | None:
    """The signal number of a stop reply (`S05`, `T05...`), or None if `reply` is not one."""
    if len(reply) >= 3 and reply[:1] in (b"S", b"T"):
        try:
            return int(reply[1:3], 16)
        except ValueError:
            return None
    return None


class GdbClient:
    """One connection to a GDB stub. Methods raise ProtocolError or OSError on failure."""

    def __init__(self, sock: socket.socket):
        self.sock = sock
        self.buffer = b""
        self.quickack = hasattr(socket, "TCP_QUICKACK") and sock.family in (socket.AF_INET, socket.AF_INET6)

    @classmethod
    def connect(cls, port: int, host: str = "127.0.0.1", timeout: float = 10.0) -> "GdbClient":
        """Connects, retrying until `timeout` while the emulator starts up."""
        deadline = time.monotonic() + timeout
        while True:
            try:
                sock = socket.create_connection((host, port), timeout=2)
                break
            except OSError:
                if time.monotonic() > deadline:
                    raise
                time.sleep(0.1)
        # Without TCP_NODELAY each small packet waits for the delayed ack (40 ms on Linux).
        sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        client = cls(sock)
        sock.sendall(b"+")
        client._expect_ack(timeout=5)
        return client

    def close(self) -> None:
        try:
            self.sock.close()
        except OSError:
            pass

    # Low level

    def _recv(self, timeout: float | None) -> None:
        self.sock.settimeout(timeout)
        data = self.sock.recv(65536)
        # melonDS sends its ack and its reply as two writes without TCP_NODELAY, so the
        # reply waits for our ack of the first; quick acks keep that from taking 40 ms.
        if self.quickack:
            self.sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_QUICKACK, 1)
        if not data:
            raise ProtocolError("connection closed")
        self.buffer += data

    def _expect_ack(self, timeout: float = 5) -> None:
        deadline = time.monotonic() + timeout
        while True:
            self.buffer = self.buffer.lstrip(b"-")
            if self.buffer[:1] == b"+":
                self.buffer = self.buffer[1:]
                return
            if self.buffer[:1] == b"$":
                # melonDS answers some packets without an ack first
                return
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                raise ProtocolError("no ack")
            self._recv(remaining)

    def send(self, payload: str | bytes, tries: int = 3) -> None:
        """Sends one packet and waits for its `+`; resends on `-`."""
        packet = encode(payload)
        for _ in range(tries):
            self.sock.sendall(packet)
            deadline = time.monotonic() + 5
            while True:
                if self.buffer[:1] == b"-":
                    self.buffer = self.buffer[1:]
                    break  # resend
                if self.buffer[:1] in (b"+", b"$"):
                    if self.buffer[:1] == b"+":
                        self.buffer = self.buffer[1:]
                    return
                remaining = deadline - time.monotonic()
                if remaining <= 0:
                    raise ProtocolError(f"no ack for {payload!r}")
                self._recv(remaining)
        raise ProtocolError(f"packet {payload!r} refused {tries} times")

    def receive(self, timeout: float | None = 5) -> bytes:
        """Waits for one packet, acks it and returns its payload. timeout None waits forever."""
        deadline = None if timeout is None else time.monotonic() + timeout
        while True:
            payload, rest, ok = decode(self.buffer)
            if payload is not None:
                self.buffer = rest
                self.sock.sendall(b"+" if ok else b"-")
                if ok:
                    return payload
                continue
            remaining = None if deadline is None else deadline - time.monotonic()
            if remaining is not None and remaining <= 0:
                raise TimeoutError("no packet")
            self._recv(remaining)

    def command(self, payload: str, timeout: float = 5) -> bytes:
        """Sends a packet and returns the reply. A stop reply left over from an
        interrupt that crossed a breakpoint is skipped, except for `?`."""
        self.send(payload)
        while True:
            reply = self.receive(timeout)
            if payload == "?" or parse_stop(reply) is None:
                return reply

    # Commands

    def halt_reason(self) -> int | None:
        """`?`: stops the target and returns the stop signal."""
        return parse_stop(self.command("?"))

    def registers(self) -> dict[str, int]:
        return parse_registers(self.command("g"))

    def write_register(self, name: str, value: int) -> None:
        """`P`: the value goes as 8 hex digits of little-endian bytes."""
        reply = self.command(f"P{REGISTERS.index(name):x}={value.to_bytes(4, 'little').hex()}")
        if reply != b"OK":
            raise ProtocolError(f"write {name}: {reply.decode(errors='replace')}")

    def read(self, addr: int, length: int) -> bytes:
        out = bytearray()
        while length > 0:
            n = min(length, MAX_READ)
            reply = self.command(f"m{addr:08x},{n:08x}")
            if reply[:1] == b"E" and len(reply) == 3:
                raise ProtocolError(f"read {addr:#x},{n:#x}: {reply.decode()}")
            out += bytes.fromhex(reply.decode())
            addr += n
            length -= n
        return bytes(out)

    def read_u32(self, addr: int) -> int:
        return int.from_bytes(self.read(addr, 4), "little")

    def set_breakpoint(self, addr: int) -> None:
        if self.command(f"Z0,{addr:x},4") != b"OK":
            raise ProtocolError(f"breakpoint at {addr:#x} refused")

    def clear_breakpoint(self, addr: int) -> None:
        if self.command(f"z0,{addr:x},4") != b"OK":
            raise ProtocolError(f"breakpoint at {addr:#x} not removed")

    def resume(self) -> None:
        """`c`: the stub answers with a stop reply only when the target stops again."""
        self.send("c")

    def wait_stop(self, timeout: float | None) -> int | None:
        """Waits for a stop reply. Returns its signal, or None on timeout."""
        try:
            return parse_stop(self.receive(timeout))
        except TimeoutError:
            return None

    def interrupt(self, timeout: float = 5) -> int | None:
        """Sends 0x03 to stop a running target and returns the stop signal."""
        self.sock.sendall(b"\x03")
        return self.wait_stop(timeout)

    def detach(self) -> None:
        try:
            self.send("D")
            self.receive(2)
        except (ProtocolError, TimeoutError, OSError):
            pass
        self.close()

    def kill(self) -> None:
        try:
            self.sock.sendall(encode("k"))
        except OSError:
            pass
        self.close()

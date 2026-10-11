#!/usr/bin/env python3
"""Generate C for unmatched very short functions.

    trivial.py [--dry-run] [--pattern P] [--limit N] [--batch N] [--name NAME]
               [--keep DIR] [-v]

Takes every function in the index that no C file covers (ARM, at most
MAX_INSNS instructions before the literal pool): empty functions, returned
constants, getters, tail calls, call wrappers, stores, load-modify-store,
address getters and comparisons. It lifts the instructions to a small
expression tree, renders C from it, and tries a grid of variants (return
type, inlined or temporary values, boolean form of `cond ? 1 : 0`, early
return). The first variant that passes the fast check (tools/fnindex.py) is
kept. Unless --dry-run, the winner is written to
src/game/<module>/func_XXXXXXXX.c, added with `fn.py add` and marked with
`fn.py finish`.

--pattern picks one family: empty, const, getter, tailcall, wrapper, store,
modify, addr, compare, other. --limit and --batch stop after N matches
(--batch is meant for commits of about 100 functions: write, build, commit,
run again). --dry-run compiles and checks but writes nothing under src/ or
config/. --keep DIR saves the winners there as well. --name NAME runs one
function and prints every variant it tried.

Types: fields get u8/s8/u16/s16/int from the load or store width; a value
loaded and then used as an address becomes a pointer to a local `Unk_`
struct with `pad_` bytes. Globals are declared locally (`extern`), so no
headers are created; the orchestrator merges the local structs later.

Skipped: functions with mrc, mcr, mrs, msr, swp or clz (they need `asm`),
functions with branches or loops, and identical twins of a function that is
already in C (tools/propagate.py handles those).

Based on khdays-decomp tools/auto_trivial.py
(CC0 1.0, commit ba2de370; see docs/third-party/khdays-decomp.md).
"""

import argparse
import re
import subprocess
import sys
import tempfile
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402
import fnindex  # noqa: E402

ROOT = fn.ROOT
MAX_INSNS = 9
PATTERNS = ["empty", "const", "getter", "tailcall", "wrapper", "store", "modify", "addr", "compare", "other"]
REGS = {f"r{i}": i for i in range(16)}
REGS.update({"fp": 11, "ip": 12, "sp": 13, "lr": 14, "pc": 15, "sl": 10, "sb": 9})
CONDS = "eq|ne|cs|hs|cc|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al"
INVERSE = {"eq": "ne", "ne": "eq", "cs": "cc", "cc": "cs", "hs": "lo", "lo": "hs", "hi": "ls", "ls": "hi",
           "ge": "lt", "lt": "ge", "gt": "le", "le": "gt"}
CMP_OP = {"eq": "==", "ne": "!=", "cs": ">=", "hs": ">=", "cc": "<", "lo": "<", "hi": ">", "ls": "<=",
          "ge": ">=", "lt": "<", "gt": ">", "le": "<="}
UNSIGNED = {"cs", "hs", "cc", "lo", "hi", "ls"}
BINOPS = {"add": "+", "sub": "-", "and": "&", "orr": "|", "eor": "^", "mul": "*"}
SHIFTS = {"lsl": "<<", "lsr": ">>", "asr": ">>"}
MEM = re.compile(rf"^(ldr|str)(sb|sh|b|h)?({CONDS})?$")
DPI = re.compile(rf"^(mov|mvn|add|sub|rsb|and|orr|eor|bic|lsl|lsr|asr|cmp|tst|mul)({CONDS})?$")
ASM_ONLY = ("mrc", "mcr", "mrs", "msr", "swp", "clz", "cdp", "ldc", "stc")
CASTS = {("lsr", 24): "u8", ("asr", 24): "s8", ("lsr", 16): "u16", ("asr", 16): "s16"}
ISFUNC = set()
DEBUG = False


class Bail(Exception):
    pass


# ------------------------------------------------------------ expression tree

class Node:
    """A value. kind: const, sym, symoff, arg, load, op, cast, call, tern."""

    def __init__(self, kind, **kw):
        self.kind = kind
        self.uses = 0
        self.struct = None  # fields, when this value is used as a base address
        self.__dict__.update(kw)


class Struct:
    def __init__(self):
        # offset -> (size, signed, load Node | None, pointer type | None, element size or 0)
        self.fields = {}


def const(n):
    return Node("const", val=n & 0xffffffff)


class Arm:
    """State of one side of a conditional section."""

    def __init__(self, regs, written):
        self.regs = dict(regs)
        self.written = set(written)
        self.stmts = []


class Lifter:
    def __init__(self, entry, insns, pool_words):
        self.entry = entry
        self.insns = insns
        self.pool_words = pool_words  # absolute address -> (word, reloc | None)
        self.relocs = {r[0]: r for r in entry["relocs"]}
        self.syms = {}
        self.args = [Node("arg", idx=i) for i in range(4)]
        self.args_used = set()
        self.regs = {i: None for i in range(16)}
        for i in range(4):
            self.regs[i] = self.args[i]
        self.written = set()
        self.stmts = []
        self.calls = []
        self.tail = None
        self.flags = None
        self.ret = None
        self.stores = 0
        self.loads = 0
        self.has_cmp = False
        self.loadnodes = []
        self.cmpnodes = []
        self.arm = None  # (primary cond, flags, base regs, base written, Arm A, Arm B)
        self.sections = []

    # --- state
    def symnode(self, name, addr, isfunc):
        if name not in self.syms:
            self.syms[name] = Node("sym", name=name, addr=addr, isfunc=isfunc)
        return self.syms[name]

    def get(self, r):
        v = self.regs.get(r)
        if v is None:
            raise Bail("garbage register")
        if v.kind == "arg":
            self.args_used.add(v.idx)
        return v

    def put(self, r, v):
        self.regs[r] = v
        if r < 4:
            self.written.add(r)

    def operand(self, t):
        t = t.strip()
        if t.startswith("#"):
            return const(int(re.split(r"[;@\s]", t[1:])[0], 0))
        if t in REGS:
            return self.get(REGS[t])
        raise Bail("operand")

    def operand2(self, text):
        parts = [p.strip() for p in text.split(",")]
        if len(parts) == 1:
            return self.operand(parts[0])
        sh = re.fullmatch(r"(lsl|lsr|asr)\s+#(\d+)", parts[1])
        if sh:
            return self.shift(sh.group(1), self.operand(parts[0]), const(int(sh.group(2))))
        raise Bail("operand2")

    def shift(self, op, a, b):
        a.uses += 1
        b.uses += 1
        if b.kind == "const" and (op, b.val) in CASTS and a.kind == "op" and a.op == "<<" \
                and a.b.kind == "const" and a.b.val == b.val:
            return Node("cast", a=a.a, ty=CASTS[(op, b.val)])
        return Node("op", op=SHIFTS[op], a=a, b=b, shift=op)

    # --- conditional sections
    def finish_arm(self):
        """Merge the two sides of a conditional section into the main state."""
        cond, flags, base_regs, base_written, A, B = self.arm
        self.arm = None
        regs = dict(base_regs)
        for r in range(16):
            a, b = A.regs[r], B.regs[r]
            if a is b:
                regs[r] = a
            elif a is None or b is None:
                regs[r] = None
            else:
                regs[r] = Node("tern", cond=cond, flags=flags, a=a, b=b)
                a.uses += 1
                b.uses += 1
        self.regs = regs
        self.written = base_written | A.written | B.written
        if A.stmts or B.stmts:
            self.stmts.append(("if", cond, flags, A.stmts, B.stmts))
        self.sections.append((cond, flags))

    def route(self, cond):
        """The side that `cond` selects, or None for an unconditional instruction."""
        if cond is None:
            if self.arm:
                raise Bail("unconditional inside a conditional section")
            return None
        if self.flags is None:
            raise Bail("condition without flags")
        if self.arm is None:
            if self.sections:
                raise Bail("two conditional sections")
            self.arm = (cond, self.flags, dict(self.regs), set(self.written),
                        Arm(self.regs, self.written), Arm(self.regs, self.written))
        primary = self.arm[0]
        if cond == primary:
            return self.arm[4]
        if cond == INVERSE.get(primary):
            return self.arm[5]
        raise Bail("unrelated condition")

    def enter(self, side):
        """Work on one side; returns what leave() needs."""
        if side is None:
            return None
        saved = (self.regs, self.written, self.stmts)
        self.regs, self.written, self.stmts = side.regs, side.written, side.stmts
        return saved

    def leave(self, side, saved):
        if side is not None:
            side.regs, side.written, side.stmts = self.regs, self.written, self.stmts
            self.regs, self.written, self.stmts = saved

    # --- running
    def run(self):
        for off, mn, ops in self.insns:
            ops = re.split(r"[;@]", ops)[0].strip()
            if mn.startswith(ASM_ONLY):
                raise Bail("asm-only")
            if mn == "bx":
                reg = REGS.get(ops)
                if self.arm:
                    self.finish_arm()
                if reg == 14:
                    self.ret = self.regs[0] if 0 in self.written else None
                    return
                if reg == 12 and self.regs[12] is not None and self.regs[12].kind == "sym" and self.regs[12].isfunc:
                    self.tail = self.make_call(self.regs[12], tail=True)
                    return
                raise Bail("bx")
            if mn in ("push", "stmfd", "pop", "ldmfd"):
                if mn in ("stmfd", "ldmfd") and not ops.startswith("sp!"):
                    raise Bail("stm")
                regs = re.search(r"\{([^}]*)\}", ops).group(1).replace(" ", "").split(",")
                if mn in ("push", "stmfd") and "pc" in regs:
                    raise Bail("push pc")
                if mn in ("pop", "ldmfd") and "pc" in regs:
                    if self.arm:
                        self.finish_arm()
                    self.ret = self.regs[0] if 0 in self.written else None
                    return
                continue
            if mn in ("sub", "add") and ops.startswith("sp, sp"):
                continue
            if mn == "bl":
                r = self.relocs.get(off)
                if not r or r[1] not in fn.CALL_KINDS:
                    raise Bail("bl without reloc")
                if self.arm:
                    raise Bail("call in a conditional section")
                self.make_call(self.symnode(r[4], r[2], True))
                continue
            if re.fullmatch(rf"b({CONDS})?|bx({CONDS})|blx.*|bl({CONDS})", mn):
                raise Bail("branch")
            m = MEM.match(mn)
            if m:
                self.memop(off, m, ops)
                continue
            m = DPI.match(mn)
            if m:
                self.dp(m.group(1), m.group(2), ops)
                continue
            raise Bail(f"insn {mn}")
        raise Bail("no return")

    def make_call(self, tgt, tail=False):
        hi = max([r for r in self.written if r < 4], default=-1)
        args = []
        for i in range(hi + 1):
            v = self.regs[i]
            if v is None:
                raise Bail("garbage argument")
            if v.kind == "arg":
                self.args_used.add(v.idx)
            v.uses += 1
            args.append(v)
        call = Node("call", target=tgt, args=args, tail=tail)
        if not tail:
            self.calls.append(call)
            self.stmts.append(("call", call))
            for i in range(1, 4):
                self.regs[i] = None
            self.regs[12] = None
            self.regs[0] = call
            self.written = {0}
        return call

    def mem(self, text):
        """[rn], [rn, #imm], [rn, rm], [rn, rm, lsl #k] -> (base, offset, index node | None, element size)."""
        t = text.strip()
        m = re.fullmatch(r"\[(\w+)(?:, #(-?\w+))?\]", t)
        if m and m.group(1) in REGS:
            return self.get(REGS[m.group(1)]), int(m.group(2) or "0", 0), None, 0
        m = re.fullmatch(r"\[(\w+), (\w+)(?:, lsl #(\d))?\]", t)
        if m and m.group(1) in REGS and m.group(2) in REGS:
            return self.get(REGS[m.group(1)]), 0, self.get(REGS[m.group(2)]), 1 << int(m.group(3) or 0)
        raise Bail("addressing mode")

    def memop(self, off, m, ops):
        kind, size, cond = m.groups()
        cond = None if cond == "al" else cond
        rd_s, _, rest = ops.partition(",")
        rd = REGS[rd_s.strip()]
        rest = rest.strip()
        if kind == "ldr" and rest.startswith("[pc"):
            mm = re.fullmatch(r"\[pc(?:, #(-?\d+))?\]", rest)
            if not mm or size or cond:
                raise Bail("pool load form")
            word, reloc = self.pool_words[off + 8 + int(mm.group(1) or 0)]
            if reloc:
                if word != reloc[2] or "+" in reloc[4] or "-" in reloc[4]:
                    raise Bail("reloc with addend")
                self.put(rd, self.symnode(reloc[4], reloc[2], reloc[4] in ISFUNC))
            else:
                self.put(rd, const(word))
            return
        side = self.route(cond)
        saved = self.enter(side)
        try:
            self.memop_body(kind, size, rd, rest)
        finally:
            self.leave(side, saved)

    def memop_body(self, kind, size, rd, rest):
        base, o, idx, arr = self.mem(rest)
        sz = {None: 4, "b": 1, "sb": 1, "h": 2, "sh": 2}[size]
        sg = size in ("sb", "sh")
        if o % sz or o < 0:
            raise Bail("unaligned")
        if idx is not None:
            if arr != sz:
                raise Bail("element size")
            idx.uses += 1
        while idx is None and base.kind == "op" and base.op == "+" and base.b.kind == "const" and base.a.kind != "const":
            o += base.b.val
            base = base.a
        if idx is None and base.kind == "op" and base.op == "+":
            for ptr, sh in ((base.a, base.b), (base.b, base.a)):
                if sh.kind == "op" and sh.op == "<<" and sh.b.kind == "const" and sh.b.val in (1, 2) \
                        and (1 << sh.b.val) == sz:
                    idx, arr, base = sh.a, sz, ptr
                    idx.uses += 1
                    break
        if base.kind == "symoff":
            o += base.off
            base = base.base
        if base.struct is None:
            base.struct = Struct()
        base.uses += 1
        fields = base.struct.fields
        prev = fields.get(o)
        if prev and (prev[0] != sz or prev[4] != arr):
            raise Bail("field type clash")
        if kind == "ldr":
            node = Node("load", base=base, off=o, size=sz, signed=sg, idx=idx)
            if prev and prev[2] is not None:
                raise Bail("field loaded twice")
            fields[o] = (sz, sg, node, prev[3] if prev else None, arr)
            self.loads += 1
            self.stmts.append(("ldseq", node))
            self.loadnodes.append(node)
            self.put(rd, node)
        else:
            val = self.get(rd)
            val.uses += 1
            if not prev:
                fields[o] = (sz, False, None, "void *" if val.kind in ("sym", "symoff") else None, arr)
            self.stores += 1
            self.stmts.append(("store", base, o, val, idx))

    def dp(self, op, cond, ops):
        cond = None if cond == "al" else cond
        if op in ("cmp", "tst"):
            if cond:
                raise Bail("conditional compare")
            a, _, b = ops.partition(",")
            x, y = self.operand(a), self.operand2(b)
            self.flags = (op, x, y)
            x.uses += 1
            y.uses += 1
            self.has_cmp = True
            self.cmpnodes += [x, y]
            return
        side = self.route(cond)
        saved = self.enter(side)
        try:
            self.dp_body(op, ops)
        finally:
            self.leave(side, saved)

    def dp_body(self, op, ops):
        parts = [p.strip() for p in ops.split(",")]
        rd = REGS[parts[0]]
        rest = ", ".join(parts[1:])
        if op in ("mov", "mvn"):
            v = self.operand2(rest)
            if op == "mvn":
                if v.kind != "const":
                    raise Bail("mvn")
                v = const(~v.val)
            self.put(rd, v)
            return
        if op in SHIFTS:
            self.put(rd, self.shift(op, self.operand(parts[1]), self.operand(parts[2])))
            return
        a = self.operand(parts[1])
        b = self.operand2(", ".join(parts[2:]))
        if op == "rsb":
            a, b, op = b, a, "sub"
        if op == "bic":
            if b.kind != "const":
                raise Bail("bic reg")
            b, op = const(~b.val), "and"
        if op == "add" and b.kind == "const":
            if a.kind == "sym":
                a.uses += 1
                self.put(rd, Node("symoff", base=a, off=b.val))
                return
            if a.kind == "symoff":
                self.put(rd, Node("symoff", base=a.base, off=a.off + b.val))
                return
            if a.kind == "op" and a.op == "+" and a.b.kind == "const":
                self.put(rd, Node("op", op="+", a=a.a, b=const(a.b.val + b.val)))
                return
        if a.kind in ("sym", "symoff") or b.kind in ("sym", "symoff"):
            raise Bail("symbol arithmetic")
        a.uses += 1
        b.uses += 1
        self.put(rd, Node("op", op=BINOPS[op], a=a, b=b))


# ------------------------------------------------------------ rendering

def tname(size, signed):
    return {(1, False): "u8", (1, True): "s8", (2, False): "u16", (2, True): "s16",
            (4, False): "int", (4, True): "int"}[(size, signed)]


def decl(ty, name):
    return f"{ty}{name}" if ty.endswith("*") else f"{ty} {name}"


class Render:
    """Renders one Lifter result with a set of options."""

    def __init__(self, L, fname, opts):
        self.L = L
        self.fname = fname
        self.opts = opts
        self.temps = {}
        self.ntemp = 0
        self.decl_syms = {}
        self.struct_names = {}
        self.structs = []
        self.callee_ret = {}
        self.callee_args = {}

    # types --------------------------------------------------------
    def struct_of(self, owner):
        if owner in self.struct_names:
            return self.struct_names[owner]
        tail = self.fname.split("_")[-1]
        if owner.kind == "sym":
            nm = "Unk_" + (owner.name.split("_")[-1] if re.fullmatch(r"\w+_[0-9a-f]{8}", owner.name) else owner.name)
        elif owner.kind == "arg":
            nm = f"Unk_{tail}" if owner.idx == 0 else f"Unk_{tail}_a{owner.idx}"
        else:
            nm = f"Unk_{tail}_{len(self.struct_names)}"
        base, n = nm, 2
        while nm in self.struct_names.values():
            nm = f"{base}_{n}"
            n += 1
        self.struct_names[owner] = nm
        self.structs.append(owner)
        return nm

    @staticmethod
    def single(owner):
        f = owner.struct.fields
        return len(f) == 1 and 0 in f

    def ptr_type(self, owner):
        if self.single(owner):
            return self.field_type(owner, 0) + "*"
        return self.struct_of(owner) + " *"

    def field_type(self, owner, off):
        size, signed, ld, extra, _arr = owner.struct.fields[off]
        if ld is not None and ld.struct is not None:
            return self.ptr_type(ld)
        if extra:
            return extra
        return tname(size, signed) + " "

    def tmp_type(self, n):
        if n.struct is not None:
            return self.ptr_type(n)
        if n.kind == "load":
            return tname(n.size, n.signed)
        if n.kind == "cast":
            return n.ty
        return "int"

    # expressions --------------------------------------------------
    def base_text(self, owner):
        if owner.kind == "sym":
            self.decl_syms.setdefault(owner.name, owner)
            return owner.name
        return self.value(owner)

    def lval(self, owner, off, idx=None):
        b = self.base_text(owner)
        ix = f"[{self.value(idx)}]" if idx is not None else ""
        if owner.kind in ("op", "const", "cast"):
            b = f"(({self.ptr_type(owner)})({b}))" if owner.kind != "const" else f"(({self.ptr_type(owner)}){b})"
        if owner.kind == "sym":
            return (b if self.single(owner) else f"{b}.unk_{off:x}") + ix
        if self.single(owner):
            if ix:
                return b + ix
            return f"*{b}" if re.fullmatch(r"[\w.>-]+|\(.*\)", b) else f"*({b})"
        return f"{b}->unk_{off:x}{ix}"

    def ival(self, n):
        """Text of a value used as an integer in arithmetic."""
        t = self.value(n)
        if n.kind in ("sym", "symoff") or n.struct is not None and n.kind in ("arg", "load", "call"):
            return f"(int){t}"
        if n.kind in ("const", "arg", "load", "call") or re.fullmatch(r"\w+", t):
            return t
        return f"({t})"

    def value(self, n):
        if n in self.temps:
            return self.temps[n]
        k = n.kind
        if k == "const":
            v = n.val - (1 << 32) if n.val >= 0x80000000 else n.val
            return str(v) if -0x100 < v < 0x100 else hex(v) if v >= 0 else f"-{hex(-v)}"
        if k == "arg":
            return f"a{n.idx}"
        if k == "sym":
            self.decl_syms.setdefault(n.name, n)
            if n.isfunc:
                return f"(void *){n.name}"
            return f"&{n.name}" if n.struct is not None else n.name
        if k == "symoff":
            self.decl_syms.setdefault(n.base.name, n.base)
            amp = "&" if n.base.struct is not None else ""
            return f"(u8 *){amp}{n.base.name} + 0x{n.off:x}"
        if k == "load":
            return self.lval(n.base, n.off, n.idx)
        if k == "cast":
            return f"({n.ty}){self.ival(n.a)}"
        if k == "op":
            if n.op == ">>" and n.shift == "lsr":
                return f"(u32){self.ival(n.a)} >> {self.value(n.b)}"
            if n.op in ("<<", ">>"):
                return f"{self.ival(n.a)} {n.op} {self.value(n.b)}"
            return f"{self.ival(n.a)} {n.op} {self.ival(n.b)}"
        if k == "tern":
            c = self.cond_text(n.cond, n.flags)
            if self.opts["bool"] and n.a.kind == "const" and n.b.kind == "const":
                if (n.a.val, n.b.val) == (1, 0):
                    return c
                if (n.a.val, n.b.val) == (0, 1):
                    return self.cond_text(INVERSE[n.cond], n.flags)
            return f"({c}) ? {self.ival(n.a)} : {self.ival(n.b)}"
        if k == "call":
            t = n.target
            self.decl_syms.setdefault(t.name, t)
            self.callee_args[t.name] = n.args
            return f"{t.name}({', '.join(self.value(a) for a in n.args)})"
        raise Bail("render " + k)

    def cond_text(self, cond, flags):
        kind, a, b = flags
        at, bt = self.ival(a), self.ival(b)
        if kind == "tst":
            return f"({at} & {bt}) {'==' if cond == 'eq' else '!='} 0"
        if cond in UNSIGNED:
            at, bt = f"(u32){at}", f"(u32){bt}"
        return f"{at} {CMP_OP[cond]} {bt}"

    # statements ---------------------------------------------------
    def needs_temp(self, n):
        if n.kind not in ("load", "op", "call", "tern", "cast"):
            return False
        if self.opts["temps"] == "all":
            return True
        extra = 1 if self.opts["cmpt"] and n in self.L.cmpnodes else 0
        return n.uses + extra > 1

    def pre(self, n, out, pad):
        """Emit temporaries for the values inside n that need one."""
        for child in (getattr(n, "a", None), getattr(n, "b", None), getattr(n, "idx", None)):
            if isinstance(child, Node):
                self.pre(child, out, pad)
        for child in getattr(n, "args", []):
            self.pre(child, out, pad)
        if n.kind in ("op", "cast", "tern") and n not in self.temps and self.needs_temp(n) and n.kind != "tern":
            t = self.value(n)
            name = f"t{self.ntemp}"
            self.ntemp += 1
            out.append(pad + decl(self.tmp_type(n), name) + f" = {t};")
            self.temps[n] = name

    def emit(self, stmts, ind, last_call=None):
        """Lines for a statement list."""
        out = []
        pad = "    " * ind
        for s in stmts:
            if s[0] == "ldseq":
                n = s[1]
                if self.needs_temp(n) and n not in self.temps:
                    t = self.value(n)
                    name = f"t{self.ntemp}"
                    self.ntemp += 1
                    out.append(pad + decl(self.tmp_type(n), name) + f" = {t};")
                    self.temps[n] = name
            elif s[0] == "call":
                n = s[1]
                if n is last_call:
                    self.pre(n, out, pad)
                    continue
                self.pre(n, out, pad)
                if n.uses > 0 or self.opts["temps"] == "all" and n is self.L.regs[0]:
                    t = self.value(n)
                    name = f"t{self.ntemp}"
                    self.ntemp += 1
                    self.callee_ret[n.target.name] = self.tmp_type(n)
                    out.append(pad + decl(self.tmp_type(n), name) + f" = {t};")
                    self.temps[n] = name
                else:
                    self.callee_ret.setdefault(n.target.name, "void")
                    out.append(pad + self.value(n) + ";")
            elif s[0] == "store":
                _, base, off, val, idx = s
                self.pre(val, out, pad)
                out.append(pad + f"{self.lval(base, off, idx)} = {self.value(val)};")
            elif s[0] == "if":
                _, cond, flags, A, B = s
                self.pre(flags[1], out, pad)
                self.pre(flags[2], out, pad)
                if A:
                    out.append(pad + f"if ({self.cond_text(cond, flags)}) {{")
                    out += self.emit(A, ind + 1)
                    if B:
                        out.append(pad + "} else {")
                        out += self.emit(B, ind + 1)
                    out.append(pad + "}")
                else:
                    out.append(pad + f"if ({self.cond_text(INVERSE[cond], flags)}) {{")
                    out += self.emit(B, ind + 1)
                    out.append(pad + "}")
        return out

    def body(self):
        L = self.L
        retvoid = self.opts["ret"] == "void"
        if L.tail is not None:
            retnode = None if retvoid else L.tail
        else:
            retnode = None if retvoid else L.ret
            if not retvoid and retnode is None:
                return None
        stmts = L.stmts
        last_call = None
        if retnode is not None and retnode.kind == "call" and retnode is not L.tail:
            if stmts and stmts[-1] == ("call", retnode) and retnode.uses == 0:
                last_call = retnode
            self.callee_ret[retnode.target.name] = self.tmp_type(retnode)
        if self.opts["ifret"]:
            # `if (c) { ...; return a; } ...; return b;`
            if not stmts or stmts[-1][0] != "if" or retnode is None or retnode.kind != "tern":
                return None
            _, cond, flags, A, B = stmts[-1]
            if retnode.cond != cond or retnode.flags is not flags:
                return None
            lines = self.emit(stmts[:-1], 1)
            lines.append(f"    if ({self.cond_text(cond, flags)}) {{")
            lines += self.emit(A, 2)
            lines.append(f"        return {self.ival(retnode.a)};")
            lines.append("    }")
            lines += self.emit(B, 1)
            lines.append(f"    return {self.ival(retnode.b)};")
            return lines
        lines = self.emit(stmts, 1, last_call)
        if L.tail is not None:
            self.pre(L.tail, lines, "    ")
        elif retnode is not None and last_call is None:
            self.pre(retnode, lines, "    ")
        if L.tail is not None and retvoid:
            self.callee_ret.setdefault(L.tail.target.name, "void")
            lines.append("    " + self.value(L.tail) + ";")
        elif retnode is not None:
            if retnode is L.tail:
                self.callee_ret[retnode.target.name] = self.tmp_type(retnode)
            text = self.value(retnode)
            if retnode.kind in ("sym", "symoff") and self.opts["ret"] != "void *":
                text = f"({self.opts['ret']}){text}"
            lines.append(f"    return {text};")
        return lines

    def source(self):
        L = self.L
        lines = self.body()
        if lines is None:
            return None
        nargs = (max(L.args_used) + 1) if L.args_used else 0
        params = [decl(self.ptr_type(L.args[i]), f"a{i}") if L.args[i].struct is not None else f"int a{i}"
                  for i in range(nargs)]
        for n in list(L.args) + list(L.loadnodes) + list(self.decl_syms.values()):
            if n.struct is not None:
                self.struct_of(n)
        fwd, typedefs, done = [], [], set()
        for owner in list(self.structs):
            nm = self.struct_names[owner]
            if self.single(owner) or nm in done:
                continue
            done.add(nm)
            fwd.append(f"typedef struct {nm} {nm};")
            fields = owner.struct.fields
            body = [f"struct {nm} {{"]
            pos = 0
            for off in sorted(fields):
                if off > pos:
                    body.append(f"    u8 pad_{pos:02x}[0x{off - pos:x}];")
                arr = "[1]" if fields[off][4] else ""
                body.append(f"    {self.field_type(owner, off)}unk_{off:x}{arr};")
                pos = off + fields[off][0]
            body.append("};")
            typedefs.append("\n".join(body))
        decls = {}
        for name, node in self.decl_syms.items():
            if node.kind != "sym":
                continue
            if node.isfunc:
                if name in self.callee_args:
                    ret = self.callee_ret.get(name, "void")
                    ps = ", ".join(self.arg_type(a) for a in self.callee_args[name]) or "void"
                    decls[name] = f"{decl(ret, name)}({ps});"
                else:
                    decls[name] = f"void {name}(void);"
            elif node.struct is not None:
                if self.single(node):
                    arr = "[]" if node.struct.fields[0][4] else ""
                    decls[name] = f"extern {decl(self.field_type(node, 0).rstrip(), name)}{arr};"
                else:
                    decls[name] = f"extern {self.struct_names[node]} {name};"
            else:
                decls[name] = f"extern u8 {name}[];"
        out = ['#include "types.h"', ""]
        if fwd:
            out += fwd + [""] + ["\n\n".join(typedefs), ""]
        if decls:
            out += list(decls.values()) + [""]
        out.append(f"{self.opts['ret']} {self.fname}({', '.join(params) or 'void'})")
        out.append("{")
        out += lines
        out.append("}")
        return "\n".join(out) + "\n"

    @staticmethod
    def arg_type(a):
        return "void *" if a.kind in ("sym", "symoff") or a.struct is not None and a.kind != "const" else "int"


# ------------------------------------------------------------ driving

def classify(L):
    if L.tail:
        return "tailcall"
    if L.calls:
        return "wrapper"
    if L.stores and L.loads:
        return "modify"
    if L.stores:
        return "store"
    if L.has_cmp:
        return "compare"
    r = L.ret
    if r is None:
        return "empty" if not L.stmts else "other"
    return {"const": "const", "sym": "addr", "symoff": "addr", "load": "getter"}.get(r.kind, "other")


def lift(entry, dis, raw):
    base = entry["addr"]
    relocs = {r[0]: r for r in entry["relocs"]}
    pool_words = {}
    for off in range(entry["pool"], entry["size"], 4):
        pool_words[base + off] = (int.from_bytes(raw[off:off + 4], "little"), relocs.get(off))
    insns = [(a, *dis.get(a, ("?", ""))) for a in range(base, base + entry["pool"], 4)]
    ent = dict(entry)
    ent["relocs"] = [[base + r[0], *r[1:]] for r in entry["relocs"]]
    L = Lifter(ent, insns, pool_words)
    L.run()
    return L


def variants(L):
    r = L.ret
    rets = ["void"]
    if L.tail is not None:
        rets = ["void", "int"]
    elif r is not None:
        if r.kind in ("sym", "symoff"):
            rets = ["void *", "u32", "int", "void"]
        elif r.kind == "load":
            t = tname(r.size, r.signed)
            rets = [t] + [k for k in ("int", "u32", "u8", "s8", "u16", "s16") if k != t] + ["void"]
        elif r.kind == "arg":
            rets = ["void"]
        elif r.kind == "call":
            rets = ["int", "void"]
        else:
            rets = ["int", "u32", "u8", "s8", "u16", "s16", "BOOL", "void"]
    temps = ["auto", "all"] if (L.loads or L.calls or L.sections or L.has_cmp) else ["auto"]
    bools = [False, True] if L.sections else [False]
    cmpts = [False, True] if L.has_cmp else [False]
    ifrets = [False, True] if L.sections else [False]
    for rt in rets:
        for t in temps:
            for b in bools:
                for c in cmpts:
                    for i in ifrets:
                        yield {"ret": rt, "temps": t, "bool": b, "cmpt": c, "ifret": i}


def candidates(idx):
    twins = {e["mhash"] for l in idx.by_name.values() for e in l if e["status"] == "complete"}
    rows = []
    for l in idx.by_name.values():
        for e in l:
            if e["status"] != "none" or e["mode"] != "arm":
                continue
            m = e["mnemonics"].split()
            if len(m) > MAX_INSNS or any(x.startswith(ASM_ONLY) for x in m) or e["mhash"] in twins:
                continue
            rows.append(e)
    rows.sort(key=lambda e: (e["module"], e["addr"]))
    return rows


def disassemble_all(rows):
    bins = fnindex.module_bins()
    return {(e["module"], e["addr"]): fnindex.disassemble(bins[e["module"]][0], bins[e["module"]][1],
                                                          e["addr"], e["addr"] + e["pool"], False)
            for e in rows}


def fit(entry, dis, idx, tmp, pattern=None):
    """Return (pattern, source | None, tries or reason)."""
    try:
        L = lift(entry, dis, bytes.fromhex(entry["raw"]))
    except Bail as b:
        return "other", None, f"bail: {b}"
    pat = classify(L)
    if pattern and pat != pattern:
        return pat, None, "skip pattern"
    src = tmp / f"{entry['name']}.c"
    why, tried, seen = "no variant rendered", 0, set()
    for opts in variants(L):
        try:
            text = Render(L, entry["name"], opts).source()
        except Bail as b:
            why = f"render: {b}"
            continue
        if text is None or text in seen:
            continue
        seen.add(text)
        tried += 1
        src.write_text(text)
        res = fnindex.check(idx, src, entry["name"], entry["module"], tmpdir=tmp)
        if res.ok:
            return pat, text, tried
        why = "compile error" if res.message == "compile error" else "differs"
        if DEBUG:
            print(f"--- {entry['name']} {opts}: {res.message}\n{text}{res.compiler_output[:400]}")
    return pat, None, why


def write_function(e, text):
    path = f"src/game/{e['module']}/{e['name']}.c"
    full = ROOT / path
    full.parent.mkdir(parents=True, exist_ok=True)
    full.write_text(text)
    r = subprocess.run([sys.executable, str(ROOT / "tools/fn.py"), "add", e["name"], path],
                       capture_output=True, text=True)
    if r.returncode:
        print(f"add failed for {e['name']}: {r.stderr.strip()}", file=sys.stderr)
        full.unlink()
        return False
    subprocess.run([sys.executable, str(ROOT / "tools/fn.py"), "finish", e["name"]],
                   capture_output=True, text=True, check=True)
    return True


def main() -> int:
    global DEBUG
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--pattern", choices=PATTERNS)
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--batch", type=int, default=0)
    ap.add_argument("--name")
    ap.add_argument("--keep")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()
    DEBUG = bool(args.name)

    idx = fnindex.load()
    ISFUNC.update(n for n, l in idx.by_name.items() if any(e["size"] for e in l))
    rows = candidates(idx)
    if args.name:
        rows = [e for e in rows if e["name"] == args.name]
    dis = disassemble_all(rows)
    stats, failures, matched = {}, {}, []
    cpu0, wall0 = time.process_time(), time.time()
    cap = args.limit or args.batch
    with tempfile.TemporaryDirectory() as td:
        for e in rows:
            pat, text, info = fit(e, dis[(e["module"], e["addr"])], idx, Path(td), args.pattern)
            if info == "skip pattern" or args.pattern and isinstance(info, str) and info.startswith("bail"):
                continue
            s = stats.setdefault(pat, {"candidates": 0, "matched": 0, "failed": 0})
            s["candidates"] += 1
            if text is None:
                s["failed"] += 1
                failures.setdefault(pat, {}).setdefault(info, []).append(e["name"])
                if args.verbose:
                    print(f"{e['name']} {pat}: {info}")
                continue
            s["matched"] += 1
            matched.append((e, pat, text))
            if args.verbose:
                print(f"{e['name']} {pat}: match after {info} variants")
            if cap and len(matched) >= cap:
                break
    if args.keep:
        Path(args.keep).mkdir(parents=True, exist_ok=True)
        for e, _, text in matched:
            (Path(args.keep) / f"{e['name']}.c").write_text(text)
    written = 0
    if not args.dry_run:
        for e, _, text in matched:
            written += write_function(e, text)
    print(f"{'pattern':10} {'candidates':>10} {'matched':>8} {'failed':>7}")
    for pat in PATTERNS:
        s = stats.get(pat)
        if s:
            print(f"{pat:10} {s['candidates']:>10} {s['matched']:>8} {s['failed']:>7}")
    for pat, d in failures.items():
        for why, names in sorted(d.items(), key=lambda kv: -len(kv[1])):
            print(f"  fail {pat}: {why}: {len(names)} e.g. {', '.join(names[:3])}")
    print(f"cpu {time.process_time() - cpu0:.1f}s wall {time.time() - wall0:.1f}s; wrote {written} files")
    return 0


if __name__ == "__main__":
    sys.exit(main())

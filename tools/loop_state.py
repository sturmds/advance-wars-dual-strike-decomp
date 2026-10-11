#!/usr/bin/env python3
"""State of the automated loop (docs/loop.md, .claude/skills/decomp-orchestrate).

The state lives outside git, in $LOOP_DIR (default
work/loop): state.json, journal.md, steer.md, and
results/<group>.txt, where each agent writes its RESULT blocks. Batch prompts
live next to it in loop-prompts/batchN/. Relative paths given to this tool
resolve against the directory that holds both (work).
Agents never change the state, so it cannot touch landing.

    loop_state.py lease                 take or refresh the orchestrator lease
    loop_state.py meta [key=value...]   print or set loop facts (cron job, dates)
    loop_state.py next-batch            the number the next batch gets
    loop_state.py add-batch N [DIR]     queue plan.json's groups as batch N
    loop_state.py add-job NAME (--template T | --prompt FILE) --model M
                  [--batch N] [--after A,B] [--class C] [--fill KEY=VALUE...]
    loop_state.py collect               read results/ for running and stopped groups
    loop_state.py reconcile             count each group's functions now in C
    loop_state.py status [--all]        one line per open group, then totals
    loop_state.py next K                up to K queued groups ready to start
    loop_state.py set NAME key=value... (tokens+=N adds; tries+=1 counts)
    loop_state.py tokens NAME AGENT N   an agent's token total (idempotent)
    loop_state.py plan-ahead            add a planned batch; queue a planner if needed
    loop_state.py close-batches         queue partials, renames, wrapup for finished batches
    loop_state.py claimed               functions held by open groups
    loop_state.py worktrees             worktrees safe to remove
    loop_state.py procs [--kill-waits]  hung shell processes under this session
    loop_state.py report [N]            functions and tokens per batch
    loop_state.py log TEXT              append a line to journal.md

A group is one agent's work: a plan.json entry (with functions) or a job
(planner, partial retry, renames, wrap-up, triage) with only a prompt.
Statuses: queued, running, stopped (usage limit, or lost with a dead
session: resume or relaunch), done, partial, failed. Functions count as done
by address, from the `complete` files in delinks.txt, so renames do not
matter.
"""

import argparse
import fcntl
import json
import os
import re
import subprocess
import sys
import time
from contextlib import contextmanager
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402

LOOP_DIR = Path(os.environ.get("LOOP_DIR", Path("work/loop")))
BASE = LOOP_DIR.parent
PROMPTS = BASE / "loop-prompts"
RESULTS = LOOP_DIR / "results"
STATE = LOOP_DIR / "state.json"
TEMPLATES = fn.ROOT / ".claude/skills/decomp-orchestrate/jobs"
OPEN = ("queued", "running", "stopped")
FINISHED = ("done", "partial", "failed")


def now() -> str:
    return time.strftime("%Y-%m-%d %H:%M")


def path(p: str) -> Path:
    q = Path(p).expanduser()
    return (q if q.is_absolute() else BASE / q).resolve()


def result_file(name: str) -> Path:
    return RESULTS / (name.replace("/", "-") + ".txt")


@contextmanager
def state(write: bool = True):
    LOOP_DIR.mkdir(parents=True, exist_ok=True)
    with open(LOOP_DIR / ".lock", "w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        s = json.loads(STATE.read_text()) if STATE.exists() else {}
        for k, v in (("lease", None), ("seq", 0), ("meta", {}), ("groups", {})):
            s.setdefault(k, v)
        yield s
        if write:
            tmp = STATE.with_suffix(".tmp")
            tmp.write_text(json.dumps(s, indent=1) + "\n")
            tmp.replace(STATE)


def session() -> tuple[str, int]:
    return os.environ.get("CLAUDE_CODE_SESSION_ID", "unknown"), int(os.environ.get("CLAUDE_PID", "0"))


def alive(pid: int) -> bool:
    if pid <= 0:
        return False
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    except PermissionError:
        pass
    return True


def done_addrs() -> set[tuple[str, int]]:
    out = set()
    for mod in fn.all_modules().values():  # ARM9 modules and arm7
        spans = [(s, e) for _, s, e, complete in mod.files if complete]
        for f in mod.funcs:
            if any(s <= f.addr < e for s, e in spans):
                out.add((mod.name, f.addr))
    return out


def cmd_lease(a, s):
    sid, pid = session()
    held = s.get("lease")
    if held and held["session"] != sid and alive(held["pid"]) and not a.force:
        print(f"HELD by session {held['session']} (pid {held['pid']}, since {held['since']}); not taking it")
        return 1
    if not held or held["session"] != sid:
        s["lease"] = {"session": sid, "pid": pid, "since": now()}
        # Agents started by an earlier session cannot be resumed from this one.
        lost = [n for n, g in s["groups"].items() if g["status"] == "running" and g.get("session") != sid]
        for n in lost:
            s["groups"][n]["status"] = "stopped"
            s["groups"][n]["lost"] = True
        print(f"TOOK lease for {sid}; {len(lost)} running groups from earlier sessions marked stopped (lost): {' '.join(lost)}")
        print("meta:", json.dumps(s["meta"]))
    else:
        print(f"OK lease held by this session since {held['since']}")
    s["lease"]["seen"] = now()
    return 0


def cmd_meta(a, s):
    for kv in a.pairs:
        k, v = kv.split("=", 1)
        s["meta"][k] = v
    print(json.dumps(s["meta"]))
    return 0


def next_batch(s) -> int:
    nums = [int(m.group(1)) for d in PROMPTS.glob("batch*") if (m := re.fullmatch(r"batch(\d+)", d.name))]
    nums += [g["batch"] for g in s["groups"].values() if g.get("batch") is not None]
    return max(nums, default=0) + 1


def cmd_next_batch(a, s):
    print(next_batch(s))
    return 0


def add_group(s, name, **fields):
    if name in s["groups"]:
        sys.exit(f"{name}: already in the state")
    s["seq"] += 1
    g = {"status": "queued", "tries": 0, "tokens": 0, "order": s["seq"], "after": [], "cls": None,
         "agent": None, "session": None, "funcs": [], "done": 0, "started": None, "ended": None, "result": ""}
    g.update(fields)
    s["groups"][name] = g


def cmd_add_batch(a, s):
    d = path(a.dir) if a.dir else PROMPTS / f"batch{a.n}"
    plan = json.loads((d / "plan.json").read_text())["groups"]
    order = [ln.split()[0] for ln in (d / "order.txt").read_text().splitlines()
             if ln.strip() and not ln.startswith("#") and not ln.endswith(":")] if (d / "order.txt").exists() else []
    names = [n for n in order if n in plan] + sorted(n for n in plan if n not in order)
    for n in names:
        g = plan[n]
        add_group(s, f"b{a.n}/{n}", batch=a.n, model=g["model"], prompt=str(d / f"{n}.md"),
                  funcs=[[f[0], f[1], f[3]] for f in g["funcs"]], after=g.get("after", []), cls=g.get("cls"))
    print(f"queued {len(names)} groups of batch {a.n} from {d}")
    return 0


def cmd_add_job(a, s):
    if a.template:
        text = (TEMPLATES / f"{a.template}.md").read_text()
        fills = {"N": str(a.batch)} if a.batch is not None else {}
        fills.update(kv.split("=", 1) for kv in a.fill or [])
        for k, v in fills.items():
            text = text.replace("{" + k + "}", v)
        if left := sorted(set(re.findall(r"\{([A-Z]+)\}", text))):
            sys.exit(f"{a.template}.md: no value for {', '.join(left)} (use --batch or --fill KEY=VALUE)")
        prompt = PROMPTS / (f"batch{a.batch}" if a.batch is not None else "jobs") / f"{a.name}.md"
        prompt.parent.mkdir(parents=True, exist_ok=True)
        prompt.write_text(text)
    elif a.prompt:
        prompt = path(a.prompt)
    else:
        sys.exit("add-job: give --template or --prompt")
    add_group(s, a.name, batch=a.batch, model=a.model, prompt=str(prompt), cls=a.cls or a.template,
              after=[x for x in (a.after or "").split(",") if x])
    print(f"queued job {a.name} ({prompt})")
    return 0


def parse_results(text: str):
    """[{RESULT, FUNCTION, COMMIT, LANDED, BEST, ...}] from RESULT blocks."""
    blocks, cur = [], None
    for ln in text.splitlines():
        m = re.match(r"\s*([A-Z]+):\s*(.*)", ln)
        if not m:
            continue
        k, v = m.groups()
        if k == "RESULT":
            cur = {}
            blocks.append(cur)
        if cur is not None:
            cur[k] = v.strip()
    return blocks


def cmd_collect(a, s):
    for n, g in sorted(s["groups"].items(), key=lambda x: x[1]["order"]):
        f = result_file(n)
        if g["status"] not in ("running", "stopped") or not f.exists():
            continue
        if g.get("started") and time.strftime("%Y-%m-%d %H:%M", time.localtime(f.stat().st_mtime)) < g["started"]:
            continue  # left by an earlier try
        blocks = parse_results(f.read_text())
        if not blocks:
            print(f"{n}: results file has no RESULT block")
            continue
        kinds = [(b["RESULT"].split() or ["empty"])[0].lower() for b in blocks]
        if "empty" in kinds:
            print(f"{n}: {kinds.count('empty')} RESULT block(s) with no value, counted as failed")
        good = sum(k in ("matched", "done") for k in kinds)
        landed = sorted({h for b in blocks for h in b.get("LANDED", "").split() if re.fullmatch(r"[0-9a-f]{7,40}", h)})
        g["status"] = "done" if good == len(kinds) else "partial" if good else "failed"
        g["result"] = f"{good}/{len(kinds)} landed {' '.join(h[:9] for h in landed) or 'none'}"
        g["ended"] = now()
        print(f"{n} {g['status']} {g['result']}")
        for b in blocks:
            commits = b.get("COMMIT", "none").split()
            own = [h for h in b.get("LANDED", "").split() if re.fullmatch(r"[0-9a-f]{7,40}", h)]
            if b["RESULT"].lower().startswith("matched") and commits != ["none"] and not own:
                print(f"  UNLANDED {b.get('FUNCTION', '?')}: {' '.join(commits)}")
    return 0


def cmd_reconcile(a, s):
    done = done_addrs()
    for n, g in s["groups"].items():
        if g["funcs"]:
            g["done"] = sum((m, addr) in done for m, addr, _ in g["funcs"])
            if g["status"] in ("queued", "stopped") and g["done"] == len(g["funcs"]):
                g["status"], g["result"] = "done", "all functions in C (reconcile)"
    return cmd_status(a, s)


def cmd_status(a, s):
    rows = [(n, g) for n, g in sorted(s["groups"].items(), key=lambda x: x[1]["order"])
            if a.all or g["status"] in ("running", "stopped")]
    sid = session()[0]
    for n, g in rows:
        fns = f"{g['done']}/{len(g['funcs'])}" if g["funcs"] else "job"
        lost = " lost" if g.get("lost") or (g["status"] == "stopped" and g.get("session") != sid) else ""
        age = ""
        if g["status"] == "running" and g.get("started"):
            mins = (time.time() - time.mktime(time.strptime(g["started"], "%Y-%m-%d %H:%M"))) / 60
            age = f" {mins / 60:.1f}h" + (" STUCK?" if mins > 240 else "")
        print(f"{n:28} {g['status']:8}{lost}{age} {g['model']:6} {fns:>7} tries={g['tries']} agent={g['agent']} {g['result'][:50]}")
    counts = {}
    for g in s["groups"].values():
        counts[g["status"]] = counts.get(g["status"], 0) + 1
    queued_work = sum(g["status"] == "queued" and bool(g["funcs"]) for g in s["groups"].values())
    print("totals:", " ".join(f"{k}={v}" for k, v in sorted(counts.items())),
          f"| ready: {len(ready(s))} | queued model groups: {queued_work} | next batch: {next_batch(s)}")
    return 0


def ready(s):
    st = {n: g["status"] for n, g in s["groups"].items()}
    busy = {g["cls"] for g in s["groups"].values() if g["status"] == "running" and g["cls"]}
    out = []
    for n, g in sorted(s["groups"].items(), key=lambda x: (x[1].get("batch") or 0, x[1]["order"])):
        if g["status"] != "queued" or any(st.get(d) not in FINISHED for d in g["after"]):
            continue
        if g["cls"] and g["cls"] in busy:
            continue
        if g["cls"]:
            busy.add(g["cls"])
        out.append(n)
    return out


def cmd_next(a, s):
    for n in ready(s)[: a.k]:
        g = s["groups"][n]
        print(f"{n}\t{g['model']}\t{g['prompt']}\t{result_file(n)}")
    return 0


def cmd_set(a, s):
    g = s["groups"].get(a.name) or sys.exit(f"{a.name}: not in the state")
    for kv in a.pairs:
        if "+=" in kv:
            k, v = kv.split("+=", 1)
            g[k] = g.get(k, 0) + int(v)
            continue
        k, v = kv.split("=", 1)
        g[k] = v
        if k == "status" and v == "running":
            g["started"], g["session"] = now(), session()[0]
            g.pop("lost", None)
        if k == "status" and v in FINISHED:
            g["ended"] = now()
    return 0


def cmd_tokens(a, s):
    """Tokens of one agent: setting the same notification twice counts once."""
    g = s["groups"].get(a.name) or sys.exit(f"{a.name}: not in the state")
    g.setdefault("agent_tokens", {})[a.agent] = a.n
    g["tokens"] = sum(g["agent_tokens"].values())
    print(f"{a.name} tokens {g['tokens']:,}")
    return 0


def batch_groups(s, n):
    return {k: g for k, g in s["groups"].items() if k.startswith(f"b{n}/")}


def queue_job(a, s, name, template, model, batch, after=None):
    a.name, a.template, a.prompt, a.model, a.batch, a.after, a.cls, a.fill = name, template, None, model, batch, after, None, None
    cmd_add_job(a, s)


def cmd_plan_ahead(a, s):
    """Add the groups of a finished planner's batch; queue a planner when
    fewer than `cap` model groups are queued and none is open."""
    for n, g in list(s["groups"].items()):
        b = g.get("batch")
        if g["cls"] == "plan" and g["status"] == "done" and not batch_groups(s, b):
            if (PROMPTS / f"batch{b}" / "plan.json").exists():
                a.n, a.dir = b, None
                cmd_add_batch(a, s)
            else:
                print(f"{n} done but loop-prompts/batch{b}/plan.json is missing: relaunch or triage it")
    queued = sum(g["status"] == "queued" and bool(g["funcs"]) for g in s["groups"].values())
    planning = [n for n, g in s["groups"].items() if g["cls"] == "plan" and g["status"] in OPEN]
    if queued < int(s["meta"].get("cap", 20)) and not planning:
        b = next_batch(s)
        queue_job(a, s, f"plan{b}", "plan", "opus", b)
    else:
        print(f"queued model groups {queued}, open planners: {' '.join(planning) or 'none'}")
    return 0


def cmd_close_batches(a, s):
    """Queue partials, renames and wrapup jobs for each batch whose groups
    have all finished."""
    done = None
    for b in sorted({g["batch"] for g in s["groups"].values() if g.get("batch") is not None}):
        groups = batch_groups(s, b)
        if not groups or any(g["status"] in OPEN for g in groups.values()) or f"renames{b}" in s["groups"]:
            continue
        done = done if done is not None else done_addrs()
        left = any(g["status"] == "partial" or any((m, x) not in done for m, x, _ in g["funcs"]) for g in groups.values())
        after = f"partials{b}" if f"partials{b}" in s["groups"] else None
        if left and not after:
            queue_job(a, s, f"partials{b}", "partials", "opus", b)
            after = f"partials{b}"
        queue_job(a, s, f"renames{b}", "renames", "opus", b, after)
        queue_job(a, s, f"wrapup{b}", "wrapup", "sonnet", b, f"renames{b}")
    return 0


def cmd_claimed(a, s):
    for n, g in s["groups"].items():
        if g["status"] in OPEN:
            for m, addr, name in g["funcs"]:
                print(f"{m}\t0x{addr:08x}\t{name}\t{n}")
    return 0


def cmd_worktrees(a, s):
    """Worktrees under .claude/worktrees/ whose branch is in mainline, with
    no uncommitted changes, not locked (the harness locks a live agent's
    worktree), and whose agent is not one of an open group."""
    held = {g["agent"] for g in s["groups"].values() if g["status"] in OPEN and g["agent"]}
    out = subprocess.run(["git", "worktree", "list", "--porcelain"], cwd=fn.ROOT, capture_output=True, text=True).stdout
    for chunk in out.strip().split("\n\n"):
        wt = dict((ln.split(" ", 1) + [""])[:2] for ln in chunk.splitlines())
        p, br = wt.get("worktree", ""), wt.get("branch", "").removeprefix("refs/heads/")
        if "/.claude/worktrees/" not in p or not br or "locked" in wt or Path(p).name.removeprefix("agent-") in held:
            continue
        dirty = subprocess.run(["git", "status", "--porcelain"], cwd=p, capture_output=True, text=True).stdout.strip()
        merged = subprocess.run(["git", "merge-base", "--is-ancestor", br, "mainline"], cwd=fn.ROOT).returncode == 0
        if merged and not dirty:
            print(f"{p}\t{br}")
    return 0


WAIT_LOOP = re.compile(r"\b(while|until)\b.*\b(pgrep|sleep|kill -0)\b")
LONG_JOB = re.compile(r"land_self|rebase_onto_mainline|apply_commit|propagate|trivial|permute|fn\.py|\bmake\b|ninja|flock")


def proc_table():
    """{pid: (ppid, age in seconds, command)} for this user's processes."""
    out = {}
    hz, boot = os.sysconf("SC_CLK_TCK"), time.time() - float(Path("/proc/uptime").read_text().split()[0])
    for d in Path("/proc").iterdir():
        if not d.name.isdigit():
            continue
        try:
            if d.stat().st_uid != os.getuid():
                continue
            stat = (d / "stat").read_text()
            fields = stat[stat.rindex(")") + 2:].split()
            cmd = (d / "cmdline").read_bytes().replace(b"\0", b" ").decode(errors="replace").strip()
        except OSError:
            continue
        out[int(d.name)] = (int(fields[1]), time.time() - (boot + int(fields[19]) / hz), cmd)
    return out


def cmd_procs(a, s):
    """Shell processes under this Claude session that look hung: wait loops
    (while/until around pgrep, sleep or kill -0) older than 30 minutes, which
    --kill-waits stops, and build or landing commands older than 90 minutes,
    which are only listed."""
    root = int(os.environ.get("CLAUDE_PID", "0"))
    procs = proc_table()

    def under(pid):
        seen = set()
        while pid > 1 and pid not in seen:
            seen.add(pid)
            if pid == root:
                return True
            pid = procs.get(pid, (0,))[0]
        return False

    found = 0
    for pid, (ppid, age, cmd) in sorted(procs.items(), key=lambda x: -x[1][1]):
        if pid == root or not under(pid) or procs.get(ppid, (0, 0, ""))[2] and WAIT_LOOP.search(procs[ppid][2]):
            continue  # report a wait loop once, not its sleep child
        body = cmd.split("eval ", 1)[-1][:160]
        if WAIT_LOOP.search(cmd) and age > 1800:
            found += 1
            if a.kill_waits:
                os.kill(pid, 15)
            print(f"WAIT {'killed ' if a.kill_waits else ''}{pid} {age / 3600:.1f}h {body}")
        elif LONG_JOB.search(cmd) and age > 5400:
            found += 1
            print(f"LONG {pid} {age / 3600:.1f}h {body}")
    print(f"procs: {found} flagged")
    return 0


def cmd_report(a, s):
    done = done_addrs()
    by = {}
    for g in s["groups"].values():
        b = by.setdefault(g.get("batch"), {"groups": 0, "open": 0, "funcs": 0, "done": 0, "tokens": 0, "models": {}})
        b["groups"] += 1
        b["open"] += g["status"] in OPEN
        b["funcs"] += len(g["funcs"])
        b["done"] += sum((m, addr) in done for m, addr, _ in g["funcs"])
        b["tokens"] += g["tokens"]
        b["models"][g["model"]] = b["models"].get(g["model"], 0) + g["tokens"]
    for b, r in sorted(by.items(), key=lambda x: (x[0] is None, x[0] or 0)):
        if a.n is not None and b != a.n:
            continue
        rate = r["done"] / r["tokens"] * 1e6 if r["tokens"] else 0
        models = ", ".join(f"{m} {t:,}" for m, t in sorted(r["models"].items()))
        print(f"batch {b}: groups {r['groups']} (open {r['open']}), planned functions in C {r['done']}/{r['funcs']}, "
              f"tokens {r['tokens']:,} ({models}), {rate:.0f} per 1M tokens")
    return 0


def cmd_log(a, s):
    with open(LOOP_DIR / "journal.md", "a") as f:
        f.write(f"- {now()} {a.text}\n")
    return 0


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    sub.add_parser("lease").add_argument("--force", action="store_true")
    sub.add_parser("meta").add_argument("pairs", nargs="*")
    sub.add_parser("next-batch")
    x = sub.add_parser("add-batch")
    x.add_argument("n", type=int)
    x.add_argument("dir", nargs="?")
    x = sub.add_parser("add-job")
    x.add_argument("name")
    x.add_argument("--template", choices=["plan", "partials", "renames", "wrapup", "triage"])
    x.add_argument("--prompt")
    x.add_argument("--model", required=True)
    x.add_argument("--batch", type=int)
    x.add_argument("--after")
    x.add_argument("--class", dest="cls")
    x.add_argument("--fill", nargs="*")
    sub.add_parser("collect")
    sub.add_parser("reconcile").add_argument("--all", action="store_true")
    sub.add_parser("status").add_argument("--all", action="store_true")
    sub.add_parser("next").add_argument("k", type=int)
    x = sub.add_parser("set")
    x.add_argument("name")
    x.add_argument("pairs", nargs="+")
    x = sub.add_parser("tokens")
    x.add_argument("name")
    x.add_argument("agent")
    x.add_argument("n", type=int)
    sub.add_parser("plan-ahead")
    sub.add_parser("close-batches")
    sub.add_parser("claimed")
    sub.add_parser("worktrees")
    sub.add_parser("procs").add_argument("--kill-waits", action="store_true")
    sub.add_parser("report").add_argument("n", type=int, nargs="?")
    sub.add_parser("log").add_argument("text")
    a = p.parse_args()
    handler = globals()["cmd_" + a.cmd.replace("-", "_")]
    readonly = ("status", "next", "next-batch", "claimed", "worktrees", "report", "log")
    with state(write=a.cmd not in readonly and not (a.cmd == "meta" and not a.pairs)) as s:
        return handler(a, s)


if __name__ == "__main__":
    sys.exit(main())

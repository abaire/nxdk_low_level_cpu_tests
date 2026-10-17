nxdk_low_level_cpu_tests
====

Various tests for low-level, non-graphics/non-audio behavior of the Xbox CPU.
The initial focus is the x86 floating point unit (FPU); the suite is intended
to grow to cover other CPU behaviors.

***WARNING: The intent of this suite is to allow emulators to check their behavior
compared to a 1.0 Microsoft Xbox.***

***This is not an IEEE compliance suite or in any
way guaranteed to match documented requirements, it tests demonstrated behavior
on one specific piece of hardware.***

* fpu_test - tests basic single- and double-precision FPU arithmetic, plus an
  x87 `fld`/`fstp` precision-control regression test (verifies that 64-bit
  `fld qword` / `fstp qword` copies are unaffected by the FPU control word's
  precision-control field, as required by the Intel x86 Architecture Manual)
  and a single-precision (PC = 00b) arithmetic rounding test (verifies that
  `fadd`/`fsub`/`fmul`/`fdiv` with `qword` operands round the exact result
  once to 24 bits, catching emulators that round to binary64 first and then
  narrow, i.e. double rounding near halfway points).
* sse_test - tests original SSE (SSE1) single-precision floating point behavior:
  - Non-IEEE Min/Max operand rules (`minss`, `maxss`, `minps`, `maxps`) for NaNs
    and signed zeros (`+0.0` vs `-0.0`).
  - Predicated comparisons (`cmpps` 0–7) and CPU `EFLAGS` updates (`comiss`, `ucomiss`),
    including Quiet NaN signaling differences.
  - Hardware approximation instructions (`rcpss`, `rcpps`, `rsqrtss`, `rsqrtps`),
    asserting 12-bit mantissa interpolation bounds and physical Coppermine baseline bit patterns.
  - `MXCSR` control flags, Flush-to-Zero (`FTZ`) underflow flush and sticky flags,
* mmx_test - tests 64-bit MMX SIMD integer behavior and x87 aliasing:
  - Packed byte, word, and dword arithmetic with wraparound (`paddb/w/d`, `psubb/w/d`), signed saturation (`paddsb/sw`, `psubsb/sw`), and unsigned saturation (`paddusb/usw`, `psubusb/usw`).
  - Packed word multiplication (`pmullw`, `pmulhw`), and multiply-and-add (`pmaddwd`) including the Intel-documented `-32768 * -32768` overflow edge case.
  - Bitwise boolean logic (`pand`, `pandn`, `por`, `pxor`), packed equality comparisons (`pcmpeqb/w/d`), and signed greater-than comparisons (`pcmpgtb/w/d`).
  - Packed shifts (`psllw/d/q`, `psrlw/d/q`, `psraw/d`) by immediate and register counts, asserting out-of-range zeroing and sign-extension fill.
  - Data packing with saturation (`packsswb`, `packssdw`, `packuswb`) and lane interleaving unpacks (`punpcklbw/wd/dq`, `punpckhbw/wd/dq`).
  - Pentium III / SSE integer MMX extensions supported on Xbox Celeron: `pavgb/w`, `pminub/maxub`, `pminsw/maxsw`, `pmulhuw`, `pmovmskb`, `pextrw/pinsrw`, `psadbw`, `pshufw`, and `maskmovq`.
  - Architectural x87/MMX physical register aliasing (`MM0`..`MM7` to `FP0`..`FP7`), tag word modifications (non-empty tags on MMX, `0xFFFF` on `EMMS`, and `0xFF` in `FXSAVE`), data retention across `EMMS`, and stack fault detection on subsequent x87 operations without `EMMS`.
* testfloat_test - integrates Berkeley TestFloat 3 with Berkeley SoftFloat 3 as an
  IEEE 754 reference oracle to validate x87 FPU hardware compliance in the guest:
  - Validates `f32`, `f64`, and `extF80` arithmetic (`fadd`, `fsub`, `fmul`, `fdiv`, `fsqrt`),
    comparisons, and conversions using deterministic Intel-syntax inline assembly (`-masm=intel`).
  - Exercises all 4 IEEE rounding modes (Nearest/Even, MinMag/Truncate, Downward/-Inf, Upward/+Inf)
    and all 3 x87 FPU precision-control modes (24-bit single, 53-bit double, 64-bit extended).


## Headless Execution & Automated Testing

To facilitate automated test runs in tools like `xemu-test`, each test suite checks for an optional configuration file at `c:\test_suite_config.ini` upon startup.

The configuration file supports flat `KEY=VALUE` pairs (lines starting with `#` or `;` are ignored):

* `log_path`: Overrides the exact output path of the log file (e.g., `log_path = c:\test_output.json` or `log_path = e:\custom_dir\log.json`). The target drive partition is mounted automatically based on the drive letter.
* `shutdown`: Enables headless termination (`shutdown = true` or `1`/`yes`/`on`). When set, the system will flush test logs and immediately power down.

If no configuration file is present or keys are omitted, suites write to their default log paths and wait for controller input (`Black` button / Right Shoulder / Start) to exit.

### Log format

The log file is a single nested JSON document (`schema_version` 1). Every event
records the source `file` (relative to `src/`) and `line` that emitted it, plus
`t_ms`, the milliseconds since the suite started:

```json
{
  "schema_version": 1,
  "suite": "SSE",
  "events": [{"level": "info", "message": "Mounted Drive E: ...", "file": "test_runner.cpp", "line": 111, "t_ms": 0}],
  "tests": [
    {
      "name": "SSE Min/Max Non-IEEE Rules",
      "source": "sse_tests/test_sse_min_max.h",
      "started_ms": 12,
      "events": [
        {"level": "pass", "message": "minss(NaN, 1.0f) == 1.0f (0x3F800000)", "file": "sse_tests/test_sse_min_max.cpp", "line": 76, "t_ms": 13},
        {"level": "fail", "message": "...", "file": "...", "line": 90, "t_ms": 13,
         "fields": {"op": "minss", "expected": "0x7FC00000", "actual": "0x3F800000"}},
        {"scope": "Double precision (PC = 10b)", "file": "...", "line": 62, "t_ms": 14, "events": []}
      ],
      "result": "fail",
      "duration_ms": 3
    }
  ],
  "summary": {"total": 4, "passed": 3, "failed": 1},
  "duration_ms": 250
}
```

Event `level` is one of `pass`, `fail`, `info`, `error`, or `dump` (a hex dump
with `label`, `size`, and `bytes`). Register and bit-pattern values in `fields`
are hex strings (e.g. `"0x3F800000"`). The file is appended to (and closed) on
every write, so if a suite crashes the document is left truncated; the last
test/event in the file shows how far it got.


## CLion

### Building

Under Settings > Build, Execution, Deployment > CMake

- Provide the path to the nxdk toolchain file under CMake Options

  `-DCMAKE_TOOLCHAIN_FILE=$CMakeProjectDir$/third_party/nxdk/share/toolchain-nxdk.cmake`

- Optionally modify the `PATH` variable to make sure the path to your chosen version of Clang comes first.

### Debugging

#### Using [xemu](xemu.app)

1. Create a new `Embedded GDB Server` target
1. Set the Target to the `<your project name>_xiso` target
1. Set the Executable to the `<your project name>` binary (it should be the only thing in the dropdown)
1. Set `Upload executable` to `Never`
1. Set `'target remote' args` to `127.0.0.1:1234`
1. Set `GDB Server` to the path to the xemu binary
1. Set `GDB Server args` to
   `-s -S -dvd_path "$CMakeCurrentBuildDir$/xiso/$CMakeCurrentTargetName$/$CMakeCurrentTargetName$.iso"` (the `-S` is
   optional and will cause xemu to wait for the debugger to connnect)
1. Under `Advanced GDB Server Options`
1. Set "Working directory" to `$ProjectFileDir$`
1. On macOS, set "Environment variables"
   to
   `DYLD_FALLBACK_LIBRARY_PATH=/<the full path to your xemu.app bundle>/Contents/Libraries/<the architecture for your platform, e.g., arm64>`
1. Set "Reset command" to `Never`

#### Using [xbdm_gdb_bridge](https://github.com/abaire/xbdm_gdb_bridge) and a devkit/XBDM-enabled Xbox

1. Create a new `Embedded GDB Server` target
1. Set the Target to the `<your project name>_xiso` target
1. Set the Executable to the `<your project name>` binary (it should be the only thing in the dropdown)
1. Set `Upload executable` to `Never`
1. Set `'target remote' args` to `127.0.0.1:1999`
1. Set `GDB Server` to the path to the xbdm_gdb_bridge binary (e.g., `<some_path>/xbdm_gdb_bridge`)
1. Set `GDB Server args` to `<your_xbox_ip> -v3 -s -- gdb :1999 e:\$CMakeCurrentTargetName$`
1. Under `Advanced GDB Server Options`, set `Reset command` to `Never`

To perform automatic deployment after builds:

1. Under the `Before launch` section, click the `+` button and add a new `Run external tool` entry.
1. Set the `Description` field to something like `Sync program and resources to the XBOX`
1. Set the `Program` field to the path to the xbdm_gdb_bridge binary (same as above)
1. Set `Arguments` to
   `<your_xbox_ip> -- mkdir e:\$CMakeCurrentTargetName$ && %syncdir $CMakeCurrentBuildDir$/xbe/xbe_file e:\$CMakeCurrentTargetName$ -f`
1. Set `Working directory` to `$ProjectFileDir$`

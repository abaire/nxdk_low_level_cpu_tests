# Guidelines for AI Coding Agents

This repository contains low-level hardware validation tests for Xbox CPU
behavior (initially the x86 floating point unit).
When contributing to or modifying this codebase, the following practices are **mandatory**:

## 1. Documentation Requirements
- **Doxygen-style Doc Comments**: Every test class header must include Doxygen-style doc comments (`//!`) documenting:
  - High-level purpose of the test.
  - An explicit enumerated list of behaviors/invariants validated by the test.
- **Concise Implementation Comments**:
  - Do not add redundant or obvious comments that merely restate what code does.
  - Do not use decorative block banners or divide test methods into artificial "Part X" sections; if a test is validating distinct behaviors, separate them into distinct `TestCase` structs.

## 2. Code Formatting Requirements
- **Run `clang-format`**: Before completing any task, format all newly created or modified source and header files (`*.cpp`, `*.h`, `*.c`) using `clang-format`:
  ```bash
  clang-format -i <path/to/changed/file>
  ```
- Ensure changes adhere cleanly to repository indentation (2 spaces) and style.

## 3. Timing and Hardware Verification Considerations
- Avoid I/O (such as `LogMsg`, `DbgPrint`, or filesystem writes) between time-sensitive operations, as disk I/O on the Xbox FATX filesystem can take several milliseconds and introduce race conditions.

## 4. String Formatting & `printf` Limitations
- **PDCLib lacks `%f` and `%e` support**: The default C runtime printf functions (`vsnprintf`, `snprintf`, `printf`, `vprintf`) supplied by PDCLib in `nxdk` do not implement floating-point format specifiers. Using `%f` or `%e` with PDCLib silently drops the values or emits empty output.
- **Use `third_party/printf`**: For logging or screen rendering involving floating-point numbers, always link against `third_party/printf` and use `vsnprintf_` / `snprintf_` from `printf/printf.h` (or wrap via `pb_print_with_floats` in `test_util.h`).

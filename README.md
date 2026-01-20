# Bazel C++ Coverage Example

A minimal Bazel project demonstrating C++ code coverage with LLVM toolchain.

## Project Structure

```
.
├── MODULE.bazel          # Bazel module (bzlmod)
├── .bazelrc              # Bazel configuration with coverage options
├── src/
│   ├── BUILD.bazel
│   ├── calculator.h
│   └── calculator.cc
└── test/
    ├── BUILD.bazel
    └── calculator_test.cc
```

## Commands

```bash
# Build
bazel build //test:calculator_test

# Test
bazel test //test:calculator_test

# Coverage
bazel coverage --config=coverage //test:calculator_test
```

## Coverage Report

After running coverage, the LCOV report is located at:
```
bazel-out/_coverage/_coverage_report.dat
```

## Known Issue: LLVM Coverage with `--experimental_split_coverage_postprocessing`

When using LLVM-based coverage (`--experimental_use_llvm_covmap`, `--experimental_generate_llvm_lcov`) together with `--experimental_split_coverage_postprocessing`, the coverage collection fails with permission errors:

```
ln: failed to create symbolic link '.../_coverage/gcov': Permission denied
error: .../_cc_coverage.dat.data: Permission denied
external/bazel_tools/tools/test/collect_cc_coverage.sh: line 95: .../_cc_coverage.dat: Permission denied
WARNING: There was no coverage found.
```

### Root Cause

The `--experimental_split_coverage_postprocessing` flag causes coverage collection to run in a separate phase. This creates coverage output directories with restricted permissions during test execution. When `collect_cc_coverage.sh` (from `bazel_tools`) attempts to:

1. Create a symlink to `llvm-cov` at `_coverage/gcov`
2. Write coverage data to `_cc_coverage.dat`
3. Process `.profraw` files with `llvm-profdata`

...it fails because the `_coverage` directory was created with read-only permissions by the split postprocessing mechanism.

The `.profraw` files (LLVM raw coverage data) are generated correctly by the instrumented test binary, but the conversion to LCOV format fails due to these permission issues.

### Attempted Fixes That Don't Work

**`--strategy=CoveragePostprocessing=local` does NOT fix this issue.**

The permission error occurs during test execution itself (inside `collect_cc_coverage.sh`), not during the separate post-processing phase. Running post-processing locally doesn't help because the error happens before that phase.

### Workarounds

**Option 1: Disable split coverage postprocessing (Recommended)**

Remove or comment out `--experimental_split_coverage_postprocessing` from `.bazelrc`:

```bash
# build:coverage --experimental_split_coverage_postprocessing
```

This runs coverage collection in the same process as the test, avoiding the permission issues entirely.

**Option 2: Use gcov instead of LLVM coverage**

Remove LLVM-specific flags and use standard gcov-based coverage (works reliably):

```bash
# Remove these from .bazelrc:
# build:coverage --experimental_use_llvm_covmap
# build:coverage --experimental_generate_llvm_lcov
# build:coverage --action_env=BAZEL_USE_LLVM_NATIVE_COVERAGE=1
# build:coverage --action_env=CC=clang
# build:coverage --action_env=CXX=clang++
```

With gcov, coverage works correctly even with `--experimental_split_coverage_postprocessing`.

### Current Configuration

The `.bazelrc` in this repo includes the LLVM coverage flags with `--experimental_split_coverage_postprocessing` to reproduce this issue. 

**To get working coverage**, either:
- Comment out `--experimental_split_coverage_postprocessing`, OR
- Remove all LLVM-specific flags and use gcov

## Requirements

- Bazel 6.0+ (for bzlmod support)
- Clang/LLVM toolchain
- `llvm-cov` and `llvm-profdata` binaries in PATH or specified via environment variables

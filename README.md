# 🧪 µTest — Micro Unit Testing Library for C

`mutest.h` is a lightweight, **single-header** unit testing library for C, with:
- Minimal boilerplate
- Flexible assertion macros
- ANSI color output and Unicode symbols
- Optional JUnit XML export for CI

Ideal for small-to-medium C projects or embedding quick tests without external dependencies.

---

## ✨ Features

- ✅ Header-only: just include one file
- 🧪 Easy to write tests with `MUT_RUN_TEST(...)`
- 🎨 Pretty output: colors and Unicode symbols
- 🧾 Export test results in JUnit XML (`--junit`)
- 📋 Common assertions: equality, string, float, null, etc.

---

## 🚀 Quick Start

### 1. Add `mutest.h` to your project

```c
#include "mutest.h"

MUT_TEST(test_addition) {
    MUT_ASSERT_EQ(2 + 2, 4);
    MUT_PASS();
}

int main(int argc, char** argv) {
    MUT_RUN_TEST(test_addition);
    MUT_REPORT();
}
```

### 2. Compile & Run

```bash
gcc -o test_suite test_suite.c
./test_suite
```

## 🧪 Command-Line Options

| Option                      | Description                 |
| --------------------------- | --------------------------- |
| `--junit results.xml`       | Export results as JUnit XML |

## 🧱 Assertions

```c
MUT_ASSERT(msg, condition)          // Generic assertion
MUT_ASSERT_EQ(a, b)                 // Integer equality
MUT_ASSERT_NEQ(a, b)                // Integer inequality
MUT_ASSERT_STR_EQ(a, b)             // String equality
MUT_ASSERT_TRUE(expr)               // True check
MUT_ASSERT_FALSE(expr)              // False check
MUT_ASSERT_NULL(ptr)                // NULL pointer check
MUT_ASSERT_NOT_NULL(ptr)            // Non-NULL pointer check
MUT_ASSERT_FLOAT_EQ(a, b, epsilon)  // Floating-point comparison
```

## 📦 GitHub Actions Integration

Here's a basic workflow using `--junit` and GitHub test summary:

```yaml
# .github/workflows/ci.yml
name: C Tests

on: [push, pull_request]

jobs:
  build-and-test:
    runs-on: ubuntu-latest
    steps:
    - name: Checkout code
      uses: actions/checkout@v3
    - name: Install build tools
      run: sudo apt-get update && sudo apt-get install -y build-essential
    - name: Build test suite
      run: gcc -o test_suite test_suite.c
    - name: Run tests and generate JUnit report
      id: run_tests
      run: |
        ./test_suite --junit results.xml
      continue-on-error: true
    - name: Upload test report
      if: always()
      uses: actions/upload-artifact@v4
      with:
        name: test-results
        path: results.xml
    - name: Publish test results
      if: always()
      uses: mikepenz/action-junit-report@v4
      with:
        report_paths: results.xml
    - name: Fail if tests failed
      if: failure() || steps.run_tests.outcome == 'failure'
      run: exit 1
```

## 📁 Project Structure Example

```bash
.
├── mutest.h         # The test framework
├── test_suite.c      # Your test file
├── .github/workflows/ci.yml
```

## 📄 License

MIT License — do whatever you want, attribution appreciated.

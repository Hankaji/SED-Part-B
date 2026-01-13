
# Electric Motor Control and Simulation (EMCS)

This project is a C++ application built using **CMake**.  
It simulates electric motor control logic and related components.

---

## Requirements

### General
- **CMake ≥ 3.20** (or newer)
- **C++ compiler with C++17 support**

### Platform-specific
- **Linux / macOS**
  - GCC or Clang
  - Make or Ninja
- **Windows**
  - Visual Studio (MSVC) **or**
  - MinGW-w64

---

## Instruction

### BUILD AND RUN

Configure:

``` Bash
cmake -S . -B build
```

Build:

``` Bash
cmake --build build
```

Run:

Linux / macOS:

``` Bash
./build/EMCS
```

Windows:

``` Bash
build\EMCS.exe
```

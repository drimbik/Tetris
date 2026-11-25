# Console Tetris (C++)

A classic console **Tetris** game built in C++ using the Windows Console API. Features colored Tetrominoes, next-piece preview, wall-kick rotation, hard drop, and persistent high score tracking.

## Features
- **All 7 Classic Tetrominoes (`I`, `J`, `L`, `O`, `S`, `T`, `Z`)** with distinct console colors.
- **Flicker-Free Rendering**: Uses cursor positioning (`SetConsoleCursorPosition`) instead of clearing the screen on every frame.
- **Wall-Kick Rotation**: Automatically shifts pieces horizontally by 1–2 cells when rotating near walls or stacked blocks.
- **Side HUD Panel**: Displays the **Next** piece preview, current **Score**, and all-time **Record**.
- **Persistent High Score**: Automatically saves and loads the best score via `record.txt`.
- **Main Menu & Instant Restart**: Start a new game, reset the high score, or restart immediately after Game Over.

## Controls

| Key | Action |
| :--- | :--- |
| **`A`** | Move Left |
| **`D`** | Move Right |
| **`S`** | Soft Drop |
| **`W`** | Rotate 90° Clockwise |
| **`Space`** | Hard Drop |
| **`ESC`** | Return to Main Menu |
| **`R` / `M`** | Restart / Main Menu (on Game Over screen) |

> **Note:** Make sure your keyboard layout is set to **English (EN)** while playing so `W`, `A`, `S`, `D` register properly.

## Build & Run

### Requirements
- **OS:** Windows (`<windows.h>`, `<conio.h>`)
- **Compiler:** MSVC, MinGW (`g++`), or Clang

### Compile via MinGW (`g++`)
```bash
g++ main.cpp -o tetris.exe
tetris.exe
```

### Compile via MSVC
```bash
cl /EHsc main.cpp /Fe:tetris.exe
tetris.exe
```

# Terminal 2048

![build](https://github.com/ysufkaratepe/terminal-2048/actions/workflows/build.yml/badge.svg)

A colorful version of the classic **2048** puzzle game that runs in the terminal, written in C++17. Works on Windows, Linux and macOS.

<p align="center">
  <img src="docs/screenshot.svg" alt="Game screen" width="440">
</p>

> The in-game text is in Turkish.

## Features

- Colored tiles and a framed game board
- Arrow keys or WASD controls (no need to press Enter)
- Undo the last move
- High score saved to a file
- Unit tests for the game rules
- Automated build and tests on Windows, Linux and macOS with GitHub Actions

## How to play

Slide the tiles to merge equal numbers and try to reach the **2048** tile. After every move a new tile (2 or 4) appears in an empty cell. The game ends when no moves are left.

| Key | Action |
| --- | --- |
| Arrow keys / W A S D | Slide tiles |
| U | Undo last move |
| R | Start a new game |
| Q | Quit |

## Build and run

**With g++** (MinGW on Windows, Linux or macOS):

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o 2048
./2048
```

**With CMake:**

```bash
cmake -B build
cmake --build build
./build/2048          # Windows: build\Debug\2048.exe
```

Run the tests:

```bash
ctest --test-dir build --output-on-failure
```

> On Windows, Windows Terminal or the new Windows 10/11 console is recommended for colors.

## Project structure

```
include/
  Board.hpp       Game rules (sliding, merging, win/lose)
  Game.hpp        Game loop, rendering, high score
  Terminal.hpp    Hides the differences between Windows and Linux/macOS terminals
src/
  Board.cpp, Game.cpp, Terminal.cpp, main.cpp
tests/
  test_board.cpp  Unit tests for the game rules
```

The game logic (`Board`) is completely separated from the screen and keyboard, so the rules can be tested without a terminal.

## How it works

All four directions are handled by the same function: each row or column is read in the direction of the move and turned into a "slide left" problem. `slideRow` packs the tiles to the left, skipping empty cells. Each tile can merge at most once per move, so the row `[2, 2, 2, 2]` becomes `[4, 4, 0, 0]`, not `[8, 0, 0, 0]`.

## Author

**Yusuf Karatepe**, Computer Engineering student
[LinkedIn](https://www.linkedin.com/in/yusuf-karatepe-0720923a1/) · [GitHub](https://github.com/ysufkaratepe)

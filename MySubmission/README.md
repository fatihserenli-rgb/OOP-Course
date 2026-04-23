# Terminal Spreadsheet 📊

A lightweight, interactive, terminal-based spreadsheet application written in C++. It operates directly in your console using ANSI escape sequences and supports dynamic cell dependencies, mathematical range functions, and file I/O operations.

## Features

* **Interactive TUI:** Navigate the 100x100 grid smoothly using arrow keys.
* **Cell Dependencies:** Link cells using basic arithmetic operations (`+`, `-`, `*`, `/`). Dependent cells automatically update when their referenced values change.
* **Built-in Functions:** Apply mathematical operations over cell ranges:
  * `sum(A1..A10)` - Sum of the range
  * `aver(A1..A10)` - Average of the range
  * `max(A1..A10)` - Maximum value in the range
  * `min(A1..A10)` - Minimum value in the range
  * `sddev(A1..A10)` - Standard deviation of the range
* **File Management:** Save and load your spreadsheet data to/from the disk.

## Under the Hood (Implementation Details)

This project is built from scratch without heavy UI frameworks like `ncurses`. It relies on pure C++ and low-level system calls:

* **Raw Terminal Control (`AnsiTerminal`):** Bypasses standard input buffering using POSIX `<termios.h>` to capture keystrokes in real-time (non-canonical mode). UI rendering and cursor positioning are handled entirely via direct **ANSI escape sequences** (e.g., `\033[H`).
* **Reactive Cell Engine (`Cell`):** Implements a custom dependency graph. Each cell maintains vectors of its `dependencies` (cells it reads from) and `dependents` (cells that read from it). When a cell's value changes, it triggers a `notifyDependents()` cascade to recursively update the grid.
* **Dynamic Viewport Rendering (`Spreadsheet`):** Instead of redrawing the entire 100x100 grid, the rendering loop calculates a dynamic sliding viewport (`max_rows` x `max_cols`) based on the cursor position. This ensures constant O(1) rendering performance and prevents terminal overflow regardless of the overall spreadsheet size.
* **Custom Parsers & Serialization (`fileManager`):** Cell references (like `AA12`) are parsed via a custom base-26 algorithm. For file I/O, data is serialized into a lightweight, comma-separated format and reconstructed back into the 2D `std::vector` structure upon loading.

## Getting Started

### Prerequisites
* A C++ compiler (e.g., `g++` or `clang++`)
* A POSIX-compliant operating system / terminal (Linux or macOS) to process ANSI escape sequences.

### Compilation
The project is structured to be compiled easily through the `main.cpp` file (which includes the necessary components via `myincludes.h`).

Run the following command in your terminal:
```bash
g++ main.cpp -o spreadsheet

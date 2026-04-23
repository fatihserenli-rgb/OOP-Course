# OOP-Course-Project
# ANSI Terminal-Based VisiCalc Clone

This project is my solution for the CSE241 Object Oriented Programming course term project. The original assignment PDF and the interface files provided by the professor are available in the `ProvidedByProfessor` folder.

Briefly, the goal of this project is to implement a clone of the classic spreadsheet system, inspired by the version available at [VisiCalc 1981](https://www.pcjs.org/software/pcx86/app/other/visicalc/1981/).

## Project Objectives

1. **User Interface:** The interface must closely resemble the original VisiCalc program.
2. **Navigation & Input:** Users must be able to navigate freely between cells and enter values into any selected cell.
3. **Cell Dependencies:** A cell can be linked to the values of other cells (e.g., `=A1` or `=A1 + B2 - C3`). If a referenced cell's value changes, the dependent cell's value must update automatically.
4. **Range Functions:** The program must implement `SUM`, `AVER`, `STDDEV`, `MAX`, and `MIN` functions. For example, if a user enters `SUM(A1..A10)` into cell A11, it should display the sum of the values from A1 to A10 and support automatic recalculation.
5. **Save & Load:** Users must be able to save and load the spreadsheet state at any time. The `save` command writes all values to a `.csv` file, while the `load` command restores them into the program. Cell dependencies and formulas must be fully preserved during save/load operations.

## Implementation Details (Core Logic & Solutions)

This project implements a reactive spreadsheet engine from scratch. Below are the technical details of the core components developed to handle data, dependencies, and grid management:

### 1. Reactive Data Engine (`cell.cpp` / `cell.h`)
The `Cell` class is the heart of the system, designed with a **Dependency-Graph** architecture:
* **Dual-Role Management:** Each cell tracks its `dependencies` (source cells) and its `dependents` (cells that rely on it).
* **Recursive Calculation:** The `getValue()` method dynamically calculates the cell's value based on its `dependType` (e.g., `AVER`, `MAX`, `SDDEV`). If a cell's formula references other cells, it recursively fetches their latest values.
* **Notification System:** When a cell's value is updated via `setValue()`, it triggers `notifyDependents()`, ensuring that all linked formulas across the spreadsheet are synchronized.
* **Arithmetic & Range Support:** Beyond simple values, the engine supports complex range operations and basic arithmetic (`+`, `-`, `*`, `/`) between individual cell references.

### 2. Spreadsheet Management & Navigation (`spreadsheet.cpp` / `spreadsheet.h`)
The `Spreadsheet` class manages the 2D grid and user interactions:
* **Dynamic Viewport Logic:** To handle a 100x100 grid efficiently, the system calculates a localized "viewport" based on the cursor's current position. This ensures that only the relevant section of the spreadsheet is rendered, keeping performance high.
* **Coordinate Parsing:** Includes a custom mapping algorithm that converts human-readable labels (e.g., `AB12`) into 2D vector indices and vice versa.
* **Command Interpretation:** * The `=` trigger initiates a link between two specific cells using arithmetic operations.
    * The `@` trigger activates a string parser that extracts function names and range boundaries (e.g., `sum(A1..A10)`) to build multi-cell dependencies.

### 3. Data Persistence (`filemanager.cpp` / `filemanager.h`)
The `fileManager` class handles the serialization of the spreadsheet state:
* **CSV-Style Serialization:** The `save` function iterates through the 2D `std::vector<Cell>`, extracting computed values and formatting them into a standard comma-separated structure.
* **State Restoration:** The `load` function parses saved files line-by-line, populates the grid, and ensures that the spreadsheet's structure is rebuilt correctly from disk.

### 4. Entry Point & Modular Structure (`main.cpp` / `myincludes.h`)
* The application utilizes a modular inclusion pattern through `myincludes.h`, which simplifies the compilation process into a single unit while maintaining a clean separation of concerns across different logic files.
* The main loop in `main.cpp` drives the **Draw-Input-Update** cycle, maintaining a responsive experience within the terminal.

## Installation and Execution

**Note:** This is an ANSI Terminal-Based project. It requires an ANSI-compatible terminal and will not work properly in a standard legacy Windows Command Prompt (using WSL, macOS, or a Linux terminal is highly recommended).
**Note:** Usage of program is provided as a menu. After run the program you can see it.

After downloading the source files into the same directory, you can compile the project using the following command:

```bash
g++ -std=c++11 main.cpp AnsiTerminal.cpp spreadsheet.cpp filemanager.cpp cell.cpp -o program

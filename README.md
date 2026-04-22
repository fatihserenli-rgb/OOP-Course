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

## Installation and Execution

**Note:** This is an ANSI Terminal-Based project. It requires an ANSI-compatible terminal and will not work properly in a standard legacy Windows Command Prompt (using WSL, macOS, or a Linux terminal is highly recommended).
**Note:** Usage of program is provided as a menu. After run the program you can see it.

After downloading the source files into the same directory, you can compile the project using the following command:

```bash
g++ -std=c++11 main.cpp AnsiTerminal.cpp spreadsheet.cpp filemanager.cpp cell.cpp -o program

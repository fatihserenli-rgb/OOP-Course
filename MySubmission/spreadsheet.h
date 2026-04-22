#ifndef SPREADSHEET_H
#define SPREADSHEET_H
#include "filemanager.h"
#include "cell.h"
#include "AnsiTerminal.h"
class Spreadsheet{
public:
    Spreadsheet(int rows, int cols);
    void drawTable();
    bool handleInput();
private:
    int row, col; // Maximum rows and columns
    int cur_row, cur_col; // Current cursor position
    std::vector<std::vector<Cell>> cells; // 2D vector of Cell objects
    AnsiTerminal terminal;
    fileManager fm;
    const int max_cols = 20; // Max columns to display at once
    const int max_rows = 32; // Max rows to display at once
    const int cell_width = 10; // Max width of each cell on the screen
    std::string getColumnLabel(int col);
    bool parseCellLabel(const std::string &label, int &row, int &col);
};

#endif
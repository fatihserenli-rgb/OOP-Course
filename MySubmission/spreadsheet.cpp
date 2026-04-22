#include "spreadsheet.h"
Spreadsheet::Spreadsheet(int rows, int cols): row(rows), col(cols), cur_row(0), cur_col(0), cells(rows, std::vector<Cell>(cols)){}


void Spreadsheet::drawTable() {
    terminal.clearScreen();
    // Hesaplamalar
    int start_row = std::max(0, cur_row - max_rows / 2);
    int end_row = std::min(start_row + max_rows, row);
    int start_col = std::max(0, cur_col - max_cols / 2);
    int end_col = std::min(start_col + max_cols, col);

    // Sütun başlıklarını yazdır
    for (int c = start_col; c < end_col; ++c) {
        terminal.printAt(1, (c - start_col) * cell_width + 5, getColumnLabel(c));
    }
    // Satırları ve hücre değerlerini yazdır
    for (int r = start_row; r < end_row; ++r) {
        terminal.printAt(r - start_row + 2, 1, std::to_string(r + 1)); // Satır numaraları
        for (int c = start_col; c < end_col; ++c) {
            std::string value = cells[r][c].getValue();
            if (value.size() > cell_width - 3) {
                value = value.substr(0, cell_width - 3); // Taşmaları önle
            }
            terminal.printAt(r - start_row + 2, (c - start_col) * cell_width + 5, value);
        }
    }
    // Sol alt köşedeki hücre bilgisi
    terminal.printAt(max_rows + 3, 1, getColumnLabel(cur_col) + std::to_string(cur_row + 1) + ":" + cells[cur_row][cur_col].getValue() + "\nenter '=' for giving cell reference\nenter '@' for using functions\nenter 'q' for exit");
    // İmleci yazdır
    terminal.printInvertedAt(cur_row - start_row + 2, (cur_col - start_col) * cell_width + 5 + ((cells[cur_row][cur_col].getValue()).substr(0, cell_width - 3)).length(), "");
}

bool Spreadsheet::handleInput() {
    char ch = terminal.getSpecialKey();
    switch (ch) {
    case 'q':
        return true;
    case 'U': // Up
        if (cur_row > 0) --cur_row;
        break;
    case 'D': // Down
        if (cur_row < row - 1) ++cur_row;
        break;
    case 'L': // Left
        if (cur_col > 0) --cur_col;
        break;
    case 'R': // Right
        if (cur_col < col - 1) ++cur_col;
        break;
    case '\x7f': { // Delete character
        std::string value = cells[cur_row][cur_col].getValue();
        if (!value.empty()) {
            value.pop_back();
            cells[cur_row][cur_col].setValue(value);
        }
        break;
    }
    case '=': { // Dependency binding
        terminal.printAt(max_rows + 4, 1,"                                      \n                                      \n                                      \n                                      \n                                      \n                                      \n");
        terminal.printAt(max_rows + 4, 1, "Enter operation (*, /, +, -): ");
        char operation = terminal.getSpecialKey();
        dependType opType;

        // Determine the operation type
        switch (operation) {
        case '*': opType = dependType::MULT; break;
        case '/': opType = dependType::DIV; break;
        case '+': opType = dependType::SUM; break;
        case '-': opType = dependType::DIFF; break;
        default:
            terminal.printAt(max_rows + 5, 1, "Invalid operation! Press any key to continue.");
            terminal.getSpecialKey(); // Wait for any key
            return false;
        }

        terminal.printAt(max_rows + 5, 1, "Enter cell label (e.g., A12): ");
        std::string cellLabel;
        char c;
        while ((c = terminal.getSpecialKey()) != 'J') { // Gather input until Enter
            if (c == '\x7f') { // Backspace handling
                if (!cellLabel.empty()) {
                    cellLabel.pop_back();
                    terminal.printAt(max_rows + 6, 1, cellLabel + " "); // Clear and update input
                }
            } else {
                cellLabel += c;
                terminal.printAt(max_rows + 6, 1, cellLabel); // Update input on screen
            }
        }

        // Parse cell label
        int targetCol = 0, targetRow = 0;
        if (parseCellLabel(cellLabel, targetRow, targetCol)) {
            if (targetRow < row && targetCol < col) {
                // Bind the current cell to the target cell
                cells[cur_row][cur_col].set_dependency(cells[targetRow][targetCol], opType);
                terminal.printAt(max_rows + 7, 1, "Dependency set successfully! Press any key to continue.");
            } else {
                terminal.printAt(max_rows + 7, 1, "Invalid cell reference! Press any key to continue.");
            }
        } else {
            terminal.printAt(max_rows + 7, 1, "Invalid cell label! Press any key to continue.");
        }
        terminal.getSpecialKey(); // Pause to let user see the message
        break;
    }
    case '@': {
        terminal.printAt(max_rows + 4, 1,"                                      \n                                      \n                                      \n                                      \n                                      \n                                      \n");
        // Menü çıktısı
        std::string menu = 
            "Some examples for how to use functions:\n"
            "1) sum(A1..A10)\n"
            "2) aver(A1..A10)\n"
            "3) max(A1..A10)\n"
            "4) min(A1..A10)\n"
            "5) sddev(A1..A10)\n"
            "6)save\n"
            "7)load\n"
            "Enter function:";
        terminal.printAt(max_rows + 3, 1, menu);
        // Kullanıcı girdisini alma
        std::string input;
        char c;
        while ((c = terminal.getSpecialKey()) != 'J') { // Enter ile sonlandır
            if (c == '\x7f') { // Delete/backspace
                if (!input.empty()) {
                    input.pop_back();
                    terminal.printAt(max_rows + 10, 16, input + " "); // Clear sonrası yaz
                }
            } else {
                input += c;
                terminal.printAt(max_rows + 10, 16, input); // Güncel input yazdır
            }
        }
        if(input == "save"){
            terminal.printAt(max_rows + 10,1,"enter filename:    ");
            terminal.printAt(max_rows + 10,16,"");
            std::string filename;
            while ((c = terminal.getSpecialKey()) != 'J') { // Enter ile sonlandır
                if (c == '\x7f') { // Delete/backspace
                    if (!filename.empty()) {
                        filename.pop_back();
                        terminal.printAt(max_rows + 10, 16, filename + " "); // Clear sonrası yaz
                    }
                } else {
                    filename += c;
                    terminal.printAt(max_rows + 10, 16, filename); // Güncel input yazdır
                }
            }
            if(fm.save(&cells,filename)) terminal.printAt(max_rows + 10, 1, "data saved successfully! Press any key to continue.");
            else terminal.printAt(max_rows + 10, 1, "file not found! Press any key to continue.");
            c = terminal.getSpecialKey();
            break;
        } else if(input == "load"){
            terminal.printAt(max_rows + 10,1,"enter filename:    ");
            terminal.printAt(max_rows + 10,16,"");
            std::string filename;
            while ((c = terminal.getSpecialKey()) != 'J') { // Enter ile sonlandır
                if (c == '\x7f') { // Delete/backspace
                    if (!filename.empty()) {
                        filename.pop_back();
                        terminal.printAt(max_rows + 10, 16, filename + " "); // Clear sonrası yaz
                    }
                } else {
                    filename += c;
                    terminal.printAt(max_rows + 10, 16, filename); // Güncel input yazdır
                }
            }
            if(fm.load(&cells,filename)) terminal.printAt(max_rows + 10, 1, "data loaded successfully! Press any key to continue.");
            else terminal.printAt(max_rows + 10, 1, "loading failed !file not found! Press any key to continue.");
            c = terminal.getSpecialKey();
            break;
        }
        // Fonksiyon ve aralık ayrıştırma
        if (input.empty()) break;
        size_t openParen = input.find('(');
        size_t closeParen = input.find(')');
        if (openParen == std::string::npos || closeParen == std::string::npos || closeParen <= openParen + 1) {
            terminal.printAt(max_rows + 10, 1, "Invalid function format. Press any key to continue.");
            terminal.getSpecialKey();
            break;
        }

        std::string function = input.substr(0, openParen);
        std::string range = input.substr(openParen + 1, closeParen - openParen - 1);
        size_t rangeSep = range.find("..");
        if (rangeSep == std::string::npos) {
            terminal.printAt(max_rows + 10, 1, "Invalid range format. Press any key to continue.");
            terminal.getSpecialKey();
            break;
        }

        std::string startCell = range.substr(0, rangeSep);
        std::string endCell = range.substr(rangeSep + 2);

        int startRow, startCol, endRow, endCol;
        if (!parseCellLabel(startCell, startRow, startCol) || 
            !parseCellLabel(endCell, endRow, endCol) ||
            startRow > endRow || startCol > endCol) {
            terminal.printAt(max_rows + 10, 1, "Invalid cell range. Press any key to continue.");
            terminal.getSpecialKey();
            break;
        }

        // Hücreleri toplama
        std::vector<Cell*> cellRange;
        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                cellRange.push_back(&cells[r][c]);
            }
        }

        // Fonksiyon tipi belirleme
        dependType fd;
        if (function == "sum") {
            fd = dependType::FSUM;
        } else if (function == "aver") {
            fd = dependType::AVER;
        } else if (function == "max") {
            fd = dependType::MAX;
        } else if (function == "min") {
            fd = dependType::MIN;
        } else if (function == "sddev") {
            fd = dependType::SDDEV;
        } else {
            terminal.printAt(max_rows + 10, 1, "Unknown function. Press any key to continue.");
            terminal.getSpecialKey();
            break;
        }

        // Bağlantıyı kur
        cells[cur_row][cur_col].set_dependency(cellRange, fd);
        terminal.printAt(max_rows + 10, 1, "Function applied successfully! Press any key to continue.");
        terminal.getSpecialKey(); // Kullanıcının sonucu görmesi için bekle
        break;
    }
    default: { // Add character to cell value
        std::string value = cells[cur_row][cur_col].getValue();
        value += ch;
        cells[cur_row][cur_col].setValue(value);
        break;
    }
    }
    return false;
}

std::string Spreadsheet::getColumnLabel(int col) {
    std::string label;
    while (col >= 0) {
        label = char('A' + (col % 26)) + label;
        col = col / 26 - 1;
    }
    return label;
}

bool Spreadsheet::parseCellLabel(const std::string &label, int &row, int &col) {
    col = 0;
    row = 0;
    size_t i = 0;

    // Parse column part (A, B, ..., Z, AA, AB, ...)
    while (i < label.size() && std::isalpha(label[i])) {
        col = col * 26 + (std::toupper(label[i]) - 'A' + 1);
        ++i;
    }
    --col; // Adjust to 0-based index

    // Parse row part
    while (i < label.size() && std::isdigit(label[i])) {
        row = row * 10 + (label[i] - '0');
        ++i;
    }
    --row; // Adjust to 0-based index

    // Validate label
    return (i == label.size() && col >= 0 && row >= 0);
}

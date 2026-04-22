#include "spreadsheet.h" //you can include myincludes.h instead of this and just compile main.cpp (g++ main.cpp -o program)
int main() {
    Spreadsheet sheet(100, 100); // 100 row and 100 col
    while(true){
        sheet.drawTable();
        if(sheet.handleInput()) break;
    }
    return 0;
}

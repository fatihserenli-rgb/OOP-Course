#include "filemanager.h"
bool fileManager::save(std::vector <std::vector < Cell> > *vec,std::string &filename){
    std::ofstream outstream(filename.c_str());
    if(outstream.fail()) return false;
    int row = vec->size();
    int col = (*vec)[0].size();
    for(int r=0;r<row;++r){
        for(int c=0;c<col;++c){
            outstream << (*vec)[r][c].getValue();
            if(c < col-1) outstream << ",";
            else outstream << "\n";
        }
    }
    outstream.close();
    return true;
    }
bool fileManager::load(std::vector<std::vector<Cell>>* vec, std::string& filename) {
    std::ifstream instream(filename.c_str());
    if (instream.fail()) return false;
    std::string line;
    int r = 0;
    while (std::getline(instream, line) && r < vec->size()) {
        size_t start = 0, end = 0;
        int c = 0;
        while ((end = line.find(',', start)) != std::string::npos && c < (*vec)[r].size()) {
            std::string cellValue = line.substr(start, end - start); // Hücre değerini ayıkla
            (*vec)[r][c].setValue(cellValue); // Hücreye değer atama
            start = end + 1; // Bir sonraki hücreye geçiş
            ++c;
        }
        // Satırın son hücresini ekle
        if (start < line.size() && c < (*vec)[r].size()) {
            std::string cellValue = line.substr(start);
            (*vec)[r][c].setValue(cellValue);
        }
        ++r; // Sonraki satıra geç
    }
    return true;
}

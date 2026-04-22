#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <fstream>
#include <vector>
#include <string>
#include "cell.h"
class fileManager{
public:
    bool save(std::vector <std::vector < Cell> > *vec,std::string &filename);
    bool load(std::vector<std::vector<Cell>>* vec, std::string& filename);
private:
};
#endif
#ifndef CELL_H
#define CELL_H
#include <string>
#include <vector>
#include <cmath>
enum class dependType { SUM, DIFF, MULT, DIV,FREE, FOUROP,FSUM, SDDEV, AVER, MAX, MIN };
class Cell{
public:
    Cell();
    void setValue(std::string& v);
    std::string getValue();
    void set_dependency(std::vector<Cell*> c,dependType fd);
    void set_dependency(Cell &c, dependType d);
private:
    std::string value;
    dependType fdtype;
    std::vector<Cell*> dependencies;
    std::vector<dependType> dependency_types;
    std::vector<Cell*> dependents;
    void addDependent(Cell *dependent);
    void notifyDependents();
};

#endif

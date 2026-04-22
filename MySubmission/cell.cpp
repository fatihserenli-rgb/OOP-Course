#include "cell.h"
bool is_num(std::string &v) {
    char *ptr = &v[0];
    bool has_decimal = false;
    while (*ptr) {
        if (*ptr < '0' || *ptr > '9') {
            if (*ptr == '.' && !has_decimal) {
                has_decimal = true; // Ondalık sayı kontrolü
            } else if (*ptr != '-' || ptr != &v[0]) {
                return false;
            }
        }
        ++ptr;
    }
    return true;
}

double ten_pow(int x) {
    double r = 1.0;
    while (x > 0) {
        r *= 10;
        --x;
    }
    return r;
}

double to_double(std::string &v) {
    double result = 0.0;
    int len = v.length();
    bool is_negative = (v[0] == '-');
    int start_index = is_negative ? 1 : 0;
    bool decimal_found = false;
    int decimal_position = 0;

    for (int i = start_index; i < len; ++i) {
        if (v[i] == '.') {
            decimal_found = true;
            decimal_position = i;
            continue;
        }
        result = result * 10 + (v[i] - '0');
    }

    if (decimal_found) {
        result /= ten_pow(len - decimal_position - 1);
    }

    return is_negative ? -result : result;
}

Cell::Cell(){value = ""; fdtype = dependType::FREE;}


void Cell::setValue(std::string& v){
        value = v;
        fdtype = dependType::FREE;
        dependencies.clear();
        dependency_types.clear();
        notifyDependents(); // Bağımlı hücrelere bildir
    }


std::string Cell::getValue(){
    double result=0;
    double max=1;
    double sd=0;
    std::string dvalue;
    switch(fdtype){
    case dependType::FREE:
        return value;
        break;
    case dependType::FOUROP:
        for(int i=0;i<dependencies.size();++i){
            dvalue=dependencies[i]->getValue();
            switch(dependency_types[i]){
            case dependType::SUM:
                result+=to_double(dvalue);
                break;
            case dependType::DIFF:
                result-=to_double(dvalue);
                break;
            case dependType::MULT:
                max*=to_double(dvalue);
                break;
            case dependType::DIV:
                if(to_double(dvalue) == 0) {value = "0"; return "0";}
                max/=to_double(dvalue);
                break;
            }
        }
        if(dependency_types[0] == dependType::MULT || dependency_types[0] == dependType::DIV) result+=max;
        else result+=(max-1); 
        value = std::to_string(result);
        return value;
        break;
    case dependType::FSUM:
        for(int i=0;i<dependencies.size();++i){
            dvalue=dependencies[i]->getValue();
            result += to_double(dvalue);
        }
        value = std::to_string(result);
        return value;
        break;
    case dependType::SDDEV:
        for(int i=0;i<dependencies.size();++i){
            dvalue=dependencies[i]->getValue();
            result+=to_double(dvalue);
        }
        result/=dependencies.size();
        for(int i=0;i<dependencies.size();++i){
            dvalue=dependencies[i]->getValue();
            max=to_double(dvalue)-result;
            sd+=pow(max,2);
        }
        sd/=dependencies.size()-1;
        result = sqrt(sd);
        value = std::to_string(result);
        return value;
        break;
    case dependType::AVER:
        for(int i=0;i<dependencies.size();++i){
            dvalue=dependencies[i]->getValue();
            result+=to_double(dvalue);
        }
        result/=dependencies.size();
        value = std::to_string(result);
        return value;
        break;
    case dependType::MAX:
        result = -999999;
        for(int i=0;i<dependencies.size();++i){
            dvalue = dependencies[i]->getValue();
            max = to_double(dvalue);
            if(max > result){ result = max;}
        }
        value = std::to_string(result);
        return value;
        break;
    case dependType::MIN:
        result = 999999;
        for(int i=0;i<dependencies.size();++i){
            dvalue = dependencies[i]->getValue();
            max = to_double(dvalue);
            if(max < result){ result = max;}
        }
        value = std::to_string(result);
        return value;
        break;
    }
    return "0";
}

void Cell::set_dependency(std::vector<Cell*> c,dependType fd){
    dependencies = c;
    fdtype = fd;
    dependency_types.clear();
    for(int i = 0;i<c.size();++i){
        c[i]->addDependent(this);
    }
}

void Cell::set_dependency(Cell &c, dependType d) {
    fdtype = dependType::FOUROP;
    dependencies.push_back(&c);
    dependency_types.push_back(d);
    c.addDependent(this); // Ters bağımlılık ekle
}

void Cell::addDependent(Cell *dependent) {
    dependents.push_back(dependent);
}

void Cell::notifyDependents() {
    for (Cell *dependent : dependents) {
        dependent->getValue(); // Bağımlı hücrelerin değerini yeniden hesapla
    }
}

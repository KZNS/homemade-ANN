#ifndef DATASET_H
#define DATASET_H

#include "matrix.h"
#include <vector>

class Dataset
{
private:
    std::vector<Matrix> datas;
public:
    int size, row, col;
    int readin(const std::string &file_name);
};

#include "dataset.cpp"

#endif
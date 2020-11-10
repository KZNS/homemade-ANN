#ifndef DATASET_H
#define DATASET_H

#include "matrix.h"
#include <vector>

class Dataset
{
private:
    char read_char(std::ifstream &fin);
    int read_int(std::ifstream &fin);
    double read_double(std::ifstream &fin);
public:
    int size, row, col;
    std::vector<Matrix> datas;
    int readin_images(const std::string &file_name);
    int readin_labels(const std::string &file_name);
};

#include "dataset.cpp"

#endif
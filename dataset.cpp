#ifndef DATASET_CPP
#define DATASET_CPP

#include "dataset.h"
#include <fstream>

int Dataset::readin(const std::string &file_name)
{
    std::ifstream fin(file_name, std::ios::in | std::ios::binary);
    int magic_number;
    fin.read((char *)&magic_number, sizeof(magic_number));
    if (magic_number % 16 == 1)
    {
        fin.read((char *)&size, sizeof(size));
        row = 1;
        col = 1;
    }
    else
    {
        fin.read((char *)&size, sizeof(size));
        fin.read((char *)&row, sizeof(row));
        fin.read((char *)&col, sizeof(col));
    }
    char u;
    for (int i = 0; i < size; i++)
    {
        datas.push_back(Matrix(row, col));
        Matrix &data = datas[datas.size() - 1];
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                fin.read(&u, sizeof(char));
                data.data[i][j] = u;
            }
        }
    }
    return 0;
}

#endif
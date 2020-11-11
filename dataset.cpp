#ifndef DATASET_CPP
#define DATASET_CPP

#include "dataset.h"
#include <fstream>

char Dataset::read_char(std::ifstream &fin)
{
    char c;
    fin.read(&c, 1);
    return c;
}
int Dataset::read_int(std::ifstream &fin)
{
    char c[4];
    for (int i = 3; i >= 0; i--)
        fin.read(c + i, 1);
    return *((int *)c);
}
double Dataset::read_double(std::ifstream &fin)
{
    char c[4];
    for (int i = 3; i >= 0; i--)
        fin.read(c + i, 1);
    return *((double *)&c);
}
int Dataset::readin_images(const std::string &file_name)
{
    std::ifstream fin(file_name, std::ios::in | std::ios::binary);
    int magic_number;
    magic_number = read_int(fin);

    size = read_int(fin);
    row = read_int(fin);
    col = read_int(fin);
    std::cout << size << " " << row << " " << col << std::endl;

    char u;
    datas.resize(size);
    for (int i = 0; i < size; i++)
    {
        Matrix &data = datas[i];
        data.set_shape(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                u = read_char(fin);
                data.data[i][j] = u / 255.0;
            }
        }
    }
    return 0;
}
int Dataset::readin_labels(const std::string &file_name)
{
    std::ifstream fin(file_name, std::ios::in | std::ios::binary);
    int magic_number;
    magic_number = read_int(fin);

    size = read_int(fin);
    row = 10;
    col = 1;
    std::cout << size << " " << row << " " << col << std::endl;

    char u;
    datas.resize(size);
    for (int i = 0; i < size; i++)
    {
        Matrix &data = datas[i];
        data.set_shape(row, col);
        u = read_char(fin);
        data.zero();
        data.data[u][0] = 1.0;
    }
    return 0;
}
int Dataset::reshape(int n, int m)
{
    if (!((n == -1 && row * col % m == 0) ||
          (m == -1 && row * col % n == 0) ||
          n * m == row * col))
    {
        return -1;
    }
    if (n == -1)
    {
        n = row * col / m;
    }
    if (m == -1)
    {
        m = row * col / n;
    }
    row = n;
    col = m;
    for (int i = 0; i < datas.size(); i++)
    {
        if (datas[i].reshape(row, col))
        {
            return -1;
        }
    }
    return 0;
}

#endif
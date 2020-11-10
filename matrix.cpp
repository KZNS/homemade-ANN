#ifndef MATRIX_CPP
#define MATRIX_CPP

#include "matrix.h"
#include <cstdio>
#include <cstring>
#include <ctime>
#include <random>

Matrix::Matrix() : row(0), col(0), data(NULL) {}
Matrix::Matrix(int n, int m) : row(n), col(m)
{
    get_mem();
}
Matrix::Matrix(const Matrix &a) : Matrix(a.row, a.col)
{
    memcpy(a.data[0], data[0], sizeof(double) * row * col);
}
Matrix::~Matrix()
{
    rm_mem();
}
void Matrix::get_mem()
{
    data = new double *[row];
    data[0] = new double[row * col];
    for (int i = 1; i < row; i++)
    {
        data[i] = data[i - 1] + col;
    }
}
void Matrix::rm_mem()
{
    if (data != NULL)
    {
        delete[] data[0];
        delete[] data;
        data = NULL;
    }
}
bool Matrix::same_shape(const Matrix &a) const
{
    if (row == a.row && col == a.col)
        return true;
    return false;
}
bool Matrix::same_cnr(const Matrix &a) const
{
    if (col == a.row)
        return true;
    return false;
}
Matrix &Matrix::copy(const Matrix &a)
{
    if (!same_shape(a))
    {
        rm_mem();
        row = a.row;
        col = a.col;
        get_mem();
    }
    memcpy(data[0], a.data[0], sizeof(double) * row * col);
    return *this;
}
Matrix &Matrix::cut(Matrix &a)
{
    if (this == &a)
        return *this;
    rm_mem();
    row = a.row;
    col = a.col;
    data = a.data;
    a.row = 0;
    a.col = 0;
    a.data = NULL;
    return *this;
}
Matrix &Matrix::zero()
{
    memset(data[0], 0, sizeof(double) * row * col);
    return *this;
}
Matrix Matrix::operator+(const Matrix &a) const
{
    if (!same_shape(a))
    {
        return *this;
    }
    Matrix b(*this);
    b += a;
    return b;
}
Matrix Matrix::operator*(const Matrix &a) const
{
    if (!same_cnr(a))
    {
        return *this;
    }
    Matrix b(row, a.col);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < a.col; j++)
        {
            b.data[i][j] = 0;
            for (int k = 0; k < col; k++)
            {
                b.data[i][j] += data[i][k] * a.data[k][j];
            }
        }
    }
    return b;
}
Matrix &Matrix::operator+=(const Matrix &a)
{
    if (!same_shape(a))
    {
        return *this;
    }
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            data[i][j] += a.data[i][j];
        }
    }
    return *this;
}
Matrix &Matrix::operator*=(const Matrix &a)
{
    if (!same_cnr(a))
        return *this;
    Matrix b(row, a.col);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < a.col; j++)
        {
            b.data[i][j] = 0;
            for (int k = 0; k < col; k++)
            {
                b.data[i][j] += data[i][k] * a.data[k][j];
            }
        }
    }
    rm_mem();
    cut(b);
    return *this;
}
Matrix &Matrix::operator=(const Matrix &a)
{
    copy(a);
    return *this;
}
double Matrix::operator()(int i, int j) const
{
    if (0 <= i && i < row && 0 <= j && j < col)
    {
        return data[i][j];
    }
    else
    {
        return -1;
    }
}
std::istream &operator>>(std::istream &in, Matrix &m)
{
    if (m.row == 0 || m.col == 0)
        return in;
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.col; j++)
        {
            if (!(in >> m.data[i][j]))
            {
                return in;
            }
        }
    }
    return in;
}
std::ostream &operator<<(std::ostream &out, const Matrix &m)
{
    if (m.row == 0 || m.col == 0)
        return out;
    for (int i = 0; i < m.row; i++)
    {
        out << m.data[i][0];
        for (int j = 1; j < m.col; j++)
        {
            out << " " << m.data[i][j];
        }
        out << std::endl;
    }
    return out;
}
int Matrix::random()
{
    std::default_random_engine e;
    std::uniform_real_distribution<double> u(0.0, 1.0);
    e.seed(std::time(0));
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            data[i][j] = u(e);
        }
    }
    return 0;
}

#endif
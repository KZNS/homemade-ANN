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
    memcpy(data[0], a.data[0], sizeof(double) * row * col);
}
Matrix::~Matrix()
{
    rm_mem();
}
int Matrix::set_shape(int n, int m)
{
    rm_mem();
    row = n;
    col = m;
    get_mem();
    return 0;
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
        std::cout << "wrong shape in +" << std::endl;
        return *this;
    }
    Matrix b(*this);
    b += a;
    return b;
}
Matrix Matrix::operator-(const Matrix &a) const
{
    if (!same_shape(a))
    {
        std::cout << "wrong shape in -" << std::endl;
        return *this;
    }
    Matrix b(*this);
    b -= a;
    return b;
}
Matrix Matrix::operator*(const Matrix &a) const
{
    if (!same_cnr(a))
    {
        std::cout << "wrong shape in *" << std::endl;
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
Matrix Matrix::operator*(const double a) const
{
    Matrix b(*this);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            b.data[i][j] *= a;
        }
    }
    return b;
}
Matrix &Matrix::operator+=(const Matrix &a)
{
    if (!same_shape(a))
    {
        std::cout << "wrong shape in +=" << std::endl;
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
Matrix &Matrix::operator-=(const Matrix &a)
{
    if (!same_shape(a))
    {
        std::cout << "wrong shape in -=" << std::endl;
        return *this;
    }
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            data[i][j] -= a.data[i][j];
        }
    }
    return *this;
}
Matrix &Matrix::operator*=(const Matrix &a)
{
    if (!same_cnr(a))
    {
        std::cout << "wrong shape in *=" << std::endl;
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
Matrix Matrix::hadamard(const Matrix &a)
{
    if (!same_shape(a))
    {
        std::cout << "wrong shape in hadamard" << std::endl;
        return *this;
    }
    Matrix b(*this);
    b.hadamard_in(a);
    return b;
}
Matrix &Matrix::hadamard_in(const Matrix &a)
{
    if (!same_shape(a))
    {
        std::cout << "wrong shape in hadamard_in" << std::endl;
        return *this;
    }
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            data[i][j] *= a.data[i][j];
        }
    }
    return *this;
}
Matrix Matrix::T() const
{
    Matrix a(col, row);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            a.data[j][i] = data[i][j];
        }
    }
    return a;
}
Matrix Matrix::sum_by_col() const
{
    Matrix b(row, 1);
    b.zero();
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            b.data[i][0] += data[i][j];
        }
    }
    return b;
}
Matrix Matrix::adds(const Matrix &a) const
{
    if (a.col != 1 || row != a.row)
    {
        std::cout << "wrong shape!" << std::endl;
        return *this;
    }
    Matrix b(*this);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            b.data[i][j] += a.data[i][0];
        }
    }
    return b;
}
int Matrix::random()
{
    static std::default_random_engine e(std::time(0));
    std::normal_distribution<double> u(0.0, 1.0);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            data[i][j] = u(e);
        }
    }
    return 0;
}
int Matrix::reshape(int n, int m)
{
    if (data == NULL)
    {
        return -2;
    }
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
    double *tmp = data[0];
    delete[] data;
    row = n;
    col = m;
    data = new double *[n];
    data[0] = tmp;
    for (int i = 1; i < n; i++)
    {
        data[i] = data[i - 1] + col;
    }
    return 0;
}
int Matrix::print_bool(std::ostream &out) const
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            out << (data[i][j] != 0) << " ";
        }
        out << std::endl;
    }
    return 0;
}

#endif
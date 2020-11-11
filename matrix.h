#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>

class Matrix
{
private:
    void get_mem();
    void rm_mem();

public:
    int row, col;
    double **data;
    Matrix();
    Matrix(int n, int m);
    Matrix(const Matrix &a);
    ~Matrix();
    int set_shape(int n, int m);
    bool same_shape(const Matrix &a) const;
    bool same_cnr(const Matrix &a) const;
    Matrix &copy(const Matrix &a);
    Matrix &cut(Matrix &a);
    Matrix &zero();
    Matrix operator+(const Matrix &a) const;
    Matrix operator*(const Matrix &a) const;
    Matrix &operator+=(const Matrix &a);
    Matrix &operator*=(const Matrix &a);
    Matrix &operator=(const Matrix &a);
    double operator()(int i, int j) const;
    friend std::istream &operator>>(std::istream &in, Matrix &m);
    friend std::ostream &operator<<(std::ostream &out, const Matrix &m);
    int random();
    int reshape(int n, int m);
    int print_bool(std::ostream &out) const;
};
#include "matrix.cpp"
#endif
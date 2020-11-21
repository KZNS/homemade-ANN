#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "matrix.h"
#include "dataset.h"

class Network
{
private:
    std::vector<Matrix *> w, b;
    std::vector<Matrix *> z, a, delta;
    int inited;

    int random_list(std::vector<int> &ls, int n);
    int fit(const Dataset &x, const Dataset &y, int mini_batch);
    int fit(const Matrix &x, const Matrix &y, double &loss, int &acn);
    int calculate(const Matrix &x, const Matrix &y);
    int optimize(const Matrix &x, const Matrix &y);
    Matrix activation(const Matrix &a);
    Matrix d_activation(const Matrix &a);
    double get_loss(const Matrix &a, const Matrix &y);
    int get_acn(const Matrix &a, const Matrix &y);
    int evaluate(const Matrix &x, const Matrix &y, double &loss, int &acn);

public:
    int input_shape;
    std::vector<int> shape;
    int deep;

    Network();
    int set_input_shape(int size);
    int add_layer(int size);
    int init();
    int fit(const Dataset &x, const Dataset &y, int mini_batch, int epochs);
    int evaluate(const Dataset &x, const Dataset &y, double &loss, double &acc);

    int print_w(int x);
};

#include "network.cpp"

#endif
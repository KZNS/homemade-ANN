#ifndef NETWORK_CPP
#define NETWORK_CPP

#include "network.h"
#include <random>

int Network::random_list(std::vector<int> &ls, int n)
{
    ls.resize(n);
    for (int i = 0; i < n; i++)
    {
        ls[i] = i;
    }
    std::default_random_engine e(std::time(0));
    for (int i = n - 1; i > 0; i--)
    {
        std::uniform_int_distribution<int> u(0, i);
        std::swap(ls[i], ls[u(e)]);
    }
    return 0;
}
int Network::fit(const Matrix &x, const Matrix &y)
{
    z[0] = w[0] * x + b[0];
    a[0] = activation(z[0]);
    for (int i = 1; i < deep; i++)
    {
        z[i] = w[i] * a[i-1] + b[i];
        a[i] = activation(z[i]);
    }
    delta[deep-1] = (a[deep-1]-y).hadamard(d_activation(z[deep-1]));
    for (int i = deep-2; i >= 0; i--)
    {
        delta[i] = (w[i+1].T()*delta[i+1]).hadamard(d_activation(z[i]));
    }
    for (int i = 0; i < deep; i++)
    {
        b[i] -= delta[i];//可以乘学习率
    }
    w[0] -= x*delta[0];
    for (int i = 1; i < deep; i++)
    {
        w[1] -= a[i-1] * delta[i];
    }
    return 0;
}
Matrix Network::activation(const Matrix &a)
{
    Matrix b(a.row, a.col);
    for (int i = 0; i < a.row; i++)
    {
        for (int j = 0; j < a.col; j++)
        {
            b.data[i][j] = a.data[i][j] < 0 ? 0 : a.data[i][j];
        }
    }
    return b;
}
Matrix Network::d_activation(const Matrix &a)
{
    Matrix b(a.row, a.col);
    for (int i = 0; i < a.row; i++)
    {
        for (int j = 0; j < a.col; j++)
        {
            b.data[i][j] = a.data[i][j] < 0 ? 0 : 1;
        }
    }
    return b;
}
Network::Network()
{
    input_shape = 0;
    deep = 0;
}
int Network::set_input_shape(int size)
{
    input_shape = size;
    if (deep)
    {
        w[0].set_shape(shape[0], input_shape);
        w[0].random();
    }
    return 0;
}
int Network::add_layer(int size)
{
    shape.push_back(size);
    deep = shape.size();
    if (deep == 1)
    {
        w.push_back(Matrix(shape[deep - 1], input_shape));
    }
    else
    {
        w.push_back(Matrix(shape[deep - 1], shape[deep - 2]));
    }
    w[deep - 1].random();
    b.push_back(Matrix(shape[deep - 1], 1));
    b[deep - 1].random();
    return 0;
}
int Network::init()
{
    z.resize(deep);
    a.resize(deep);
    delta.resize(deep);
    return 0;
}
int Network::fit(const Dataset &x, const Dataset &y)
{
    std::vector<int> ls;
    random_list(ls, x.size);
    for (int i = 0; i < x.size; i++)
    {
        printf("\r%d/%d", i+1, x.size);
        fit(x.datas[ls[i]], y.datas[ls[i]]);
    }
    printf("\n");
    return 0;
}
int Network::fit(const Dataset &x, const Dataset &y, int epochs)
{
    for (int i = 1; i <= epochs; i++)
    {
        std::cout << "epoch " << i << std::endl;
        if (fit(x, y))
        {
            std::cout << "error in fit()" << std::endl;
            return -1;
        }
    }
    return 0;
}
int Network::evaluate(const Dataset &x, const Dataset &y, double &test_loss, double &test_acc)
{
    return 0;
}

#endif
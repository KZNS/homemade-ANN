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
int Network::fit(const Matrix &x, const Matrix &y, double &loss, int &acn)
{
    z[0] = w[0] * x + b[0];
    a[0] = activation(z[0]);
    for (int i = 1; i < deep; i++)
    {
        z[i] = w[i] * a[i - 1] + b[i];
        a[i] = activation(z[i]);
    }
    loss = get_loss(a[deep - 1], y);
    acn = get_acn(a[deep - 1], y);
    delta[deep - 1] = (a[deep - 1] - y).hadamard(d_activation(z[deep - 1]));
    for (int i = deep - 2; i >= 0; i--)
    {
        delta[i] = (w[i + 1].T() * delta[i + 1]).hadamard(d_activation(z[i]));
    }
    for (int i = 0; i < deep; i++)
    {
        b[i] -= delta[i]; //可以乘学习率
    }
    w[0] -= x * delta[0];
    for (int i = 1; i < deep; i++)
    {
        w[1] -= a[i - 1] * delta[i];
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
double Network::get_loss(const Matrix &a, const Matrix &y)
{
    double loss = 0;
    Matrix c, d;
    c = y - a;
    d = c;
    c.hadamard_in(d);
    double loss1;
    for (int j = 0; j < c.col; j++)
    {
        loss1 = 0;
        for (int i = 0; i < c.row; i++)
        {
            loss1 += c.data[i][j];
        }
        loss1 /= 2;
        if (loss)
            loss += loss1;
    }
    loss /= c.col;
    return loss;
}
int Network::get_acn(const Matrix &a, const Matrix &y)
{
    int acn = 0;
    bool f;
    for (int j = 0; j < a.col; j++)
    {
        f = true;
        for (int i = 0; j < a.row; j++)
        {
            if ((a.data[i][j] < 0.5 ? 0 : 1) != y.data[i][j])
            {
                f = false;
                break;
            }
        }
        if (f)
        {
            acn++;
        }
    }
    return acn;
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
    double acn = 0;
    double acc = 0, loss = 0;
    double loss_one;
    int acn_one;
    for (int i = 0; i < x.size; i++)
    {
        printf("\r%d/%d: loss: %.3lf acc: %.3lf",
               i + 1, x.size, loss, acc);
        fit(x.datas[ls[i]], y.datas[ls[i]], loss_one, acn_one);
        loss = (loss * i + loss_one) / (i + 1);
        acn += acn_one;
        acc = acn / (i + 1);
    }
    printf("\r%d/%d: loss: %.3lf acc: %.3lf\n", x.size, x.size, loss, acc);
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
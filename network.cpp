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
    static std::default_random_engine e(std::time(0));
    for (int i = n - 1; i > 0; i--)
    {
        std::uniform_int_distribution<int> u(0, i);
        std::swap(ls[i], ls[u(e)]);
    }
    return 0;
}
int Network::fit(const Dataset &x, const Dataset &y, int mini_batch)
{
    std::vector<int> ls;
    random_list(ls, x.size);
    int batch_n = x.size / mini_batch;

    double acn = 0;
    double acc = 0, loss = 0;
    int acn_one;

    Matrix x_batch(x.row, mini_batch);
    Matrix y_batch(y.row, mini_batch);
    int id;
    for (int i = 0; i < batch_n; i++)
    {
        printf("\r%d/%d: loss: %.3lf acc: %.3lf        ", i + 1, batch_n, loss, acc);
        for (int j = 0; j < mini_batch; j++)
        {
            id = ls[i * mini_batch + j];
            for (int k = 0; k < x.row; k++)
            {
                x_batch.data[k][j] = x.datas[id].data[k][0];
            }
            for (int k = 0; k < y.row; k++)
            {
                y_batch.data[k][j] = x.datas[id].data[k][0];
            }
        }
        fit(x_batch, y_batch, loss, acn_one);
        acn += acn_one;
        acc = acn / ((i + 1) * mini_batch);
    }
    printf("\r%d/%d: loss: %.3lf acc: %.3lf        \n", batch_n, batch_n, loss, acc);
    return 0;
}
int Network::fit(const Matrix &x, const Matrix &y, double &loss, int &acn)
{
    calculate(x, y);
    loss = get_loss(*z[deep - 1], y);
    acn = get_acn(*z[deep - 1], y);
    optimize(x, y);
    return 0;
}
int Network::calculate(const Matrix &x, const Matrix &y)
{
    *z[0] = (*w[0] * x).adds(*b[0]);
    *a[0] = activation(*z[0]);
    for (int i = 1; i < deep - 1; i++)
    {
        *z[i] = (*w[i] * *a[i - 1]).adds(*b[i]);
        *a[i] = activation(*z[i]);
    }
    *z[deep - 1] = (*w[deep - 1] * *a[deep - 2]).adds(*b[deep - 1]);
    return 0;
}
int Network::optimize(const Matrix &x, const Matrix &y)
{
    *delta[deep - 1] = *z[deep - 1] - y;
    for (int i = deep - 2; i >= 0; i--)
    {
        *delta[i] = (w[i + 1]->T() * (*delta[i + 1])).hadamard(d_activation(*z[i]));
    }
    double lam = 0.0005;
    for (int i = 0; i < deep; i++)
    {
        *b[i] -= delta[i]->sum_by_col() * (1.0 / x.col) * lam;
    }
    *w[0] -= *delta[0] * x.T() * (1.0 / x.col) * lam;
    for (int i = 1; i < deep; i++)
    {
        *w[i] -= *delta[i] * a[i - 1]->T() * (1.0 / x.col) * lam;
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
        for (int i = 0; i < a.row; i++)
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
int Network::evaluate(const Matrix &x, const Matrix &y, double &loss, int &acn)
{
    calculate(x, y);
    loss = get_loss(*z[deep - 1], y);
    acn = get_acn(*z[deep - 1], y);
    return 0;
}

Network::Network()
{
    input_shape = 0;
    deep = 0;
    inited = 0;
}
int Network::set_input_shape(int size)
{
    input_shape = size;
    if (deep)
    {
        w[0]->set_shape(shape[0], input_shape);
        w[0]->random();
    }
    return 0;
}
int Network::add_layer(int size)
{
    shape.push_back(size);
    deep = shape.size();
    if (deep == 1)
    {
        w.push_back(new Matrix(shape[deep - 1], input_shape));
    }
    else
    {
        w.push_back(new Matrix(shape[deep - 1], shape[deep - 2]));
    }
    w[deep - 1]->random();
    b.push_back(new Matrix(shape[deep - 1], 1));
    b[deep - 1]->random();
    return 0;
}
int Network::init()
{
    for (int i = 0; i < deep; i++)
    {
        z.push_back(new Matrix);
        a.push_back(new Matrix);
        delta.push_back(new Matrix);
    }
    inited = 1;
    return 0;
}
int Network::fit(const Dataset &x, const Dataset &y, int mini_batch, int epochs)
{
    if (inited != 1)
    {
        return -1;
    }
    for (int i = 1; i <= epochs; i++)
    {
        std::cout << "epoch " << i << std::endl;
        if (fit(x, y, mini_batch))
        {
            std::cout << "error in fit()" << std::endl;
            return -1;
        }
    }
    return 0;
}
int Network::fit(const Dataset &x, const Dataset &y, int mini_batch, int epochs,
                 const Dataset &x_test, const Dataset &y_test)
{
    if (inited != 1)
    {
        return -1;
    }
    double loss, acc;
    for (int i = 1; i <= epochs; i++)
    {
        std::cout << "epoch " << i << std::endl;
        if (fit(x, y, mini_batch))
        {
            std::cout << "error in fit()" << std::endl;
            return -1;
        }
        evaluate(x_test, y_test, loss, acc);
    }
    return 0;
}
int Network::evaluate(const Dataset &x, const Dataset &y, double &loss, double &acc)
{
    int acn = 0, acn_one;
    double loss_one;
    loss = 0;
    acc = 0;
    for (int i = 0; i < x.size; i++)
    {
        printf("\rtesting %d/%d", i + 1, x.size);
        evaluate(x.datas[i], y.datas[i], loss_one, acn_one);
        acn += acn_one;
        loss += loss_one;
    }
    printf("\n");
    loss /= x.size;
    acc = (double)acn / x.size;
    printf("acn: %d\n", acn);
    printf("loss: %.3lf acc: %.3lf\n", loss, acc);
    return 0;
}
int Network::print_w(int x)
{
    if (x < w.size())
    {
        std::cout << *w[x];
        return 0;
    }
    return -1;
}

#endif
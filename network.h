#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "matrix.h"

class network
{
private:
    std::vector<Matrix*> w, b;

public:
    ~network();
    std::vector<int> shape;
    int deep;
    void add_layer(int size);
};

#include "network.cpp"

#endif
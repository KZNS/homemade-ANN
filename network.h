#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "matrix.h"

class Network
{
private:
    std::vector<Matrix*> w, b;

public:
    ~Network();
    std::vector<int> shape;
    int deep;
    void add_layer(int size);
};

#include "network.cpp"

#endif
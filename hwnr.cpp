#include <cstdio>
#include "network.h"
#include "dataset.h"
using namespace std;

int main()
{
    //readin data
    network net;
    net.add_layer(28 * 28);
    net.add_layer(128);
    net.add_layer(10);
    //init network
    //fit
    //test
    return 0;
}
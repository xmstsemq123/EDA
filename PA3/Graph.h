#ifndef GRAPH_H_
#define GRAPH_H_
#include "ReadFile.h"
#include <deque>
using namespace std;
using net = ReadFile::net;
class Graph{
    // functions definition
    public:
        Graph(deque<net>* nc_);
        void draw();
    private:

    // variables definition
    private:
        deque<net>* nc;
};
#endif
#ifndef ASTARALGORITHM_H_
#define ASTARALGORITHM_H_
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include "ReadFile.h"
using namespace std;
class AStarAlgorithm{
    // namespace definition
    public:
        using p = ReadFile::p;
        using um = ReadFile::um;
        struct Node{
            int x, y;
            int g, h, c, b; // c: crossing, b: bending
            string direction; // east, south, west, north, no
            Node* parent;
            um* lossP;
            Node(int x_, int y_, int g_, int h_, int c_, int b_, string direction_, um* lossP_, Node* parent_);
            int f() const;
            bool operator > (const Node& other) const;
            // penalty parameter
            int alpha, beta, gamma; // alpha: length, beta: crossing, gamma: bending
        };
    // functions definition
    public:
        AStarAlgorithm(p grid, um* lossP_);
        vector<p> execute(p start, p target);
        void addBlock(vector<p> pathBlock);
        int heuristic(int x1, int y1, int x2, int y2);
    private:
    // variables definition
    private:
        int row, col;
        um* lossP;
        unordered_map<int, unordered_map<int, int> > block; // nx, ny, amount of crossed points
};
#endif
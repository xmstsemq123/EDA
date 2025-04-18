#ifndef _ALGORITHM1_H_
#define _ALGORITHM1_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "ReadDEF.h"
using namespace std;
class Algorithm1{
    // functions
    public:
        Algorithm1(ReadDEF *RDEFPointer);
        void Sort();
        void showSCC();
        vector< map<string, string> > *getSCCP();
    private:
        void SortByX();
        int FindNearestSiteX(int& OX, map<string, string>& row);
        int PlaceRow(map<string, string>& cell, map<string, string>& row, int cellIndex);
    // variables
    private:
        ReadDEF* RDEF;
        // components after sort
        vector< map<string, string> > SCC; // Sorted Component Container
        // the placed site in row
        map<string, vector<string> > PlacedSite; // {"Y of Row", "Set of StartX"}
};
#endif
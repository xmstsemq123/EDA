#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include "Displacement.h"
using namespace std;

Displacement::Displacement(ReadDEF* RDEF, vector< map<string, string> >* SCCP){
    this->RDEF = RDEF;
    this->SCCP = SCCP;
    NCC.resize(RDEF->ComponentContainer.size());
    copy(RDEF->ComponentContainer.begin(), RDEF->ComponentContainer.end(), NCC.begin());
    sort(NCC.begin(), NCC.end(), [](map<string, string> &a, map<string, string> &b){
        return stoi(a.at("StartX")) < stoi(b.at("StartX"));
    });
}

void Displacement::show(){
    double cellDis = 0, maxDis = 0, tempDis = 0, quality = 0;
    int ox, oy, nx, ny;
    for(int i=0; i<NCC.size(); i++){
        ox = stoi(NCC[i]["StartX"]);
        oy = stoi(NCC[i]["StartY"]);
        nx = stoi((*SCCP)[i]["StartX"]);
        ny = stoi((*SCCP)[i]["StartY"]);
        tempDis = abs(nx - ox) + abs(ny - oy);
        if(tempDis > maxDis) maxDis = tempDis;
        cellDis += tempDis;
    }
    quality = cellDis/RDEF->ComponentAmount + RDEF->alpha * maxDis;
    cout << "Solution Quality: " << quality << endl;
}
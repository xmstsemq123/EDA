#ifndef _DISPLACEMENT_H_
#define _DISPLACEMENT_H_
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include "ReadDEF.h"
using namespace std;
class Displacement{
    public:
        Displacement(ReadDEF* RDEF, vector< map<string, string> >* SCCP);
        void show();
    private:
        ReadDEF* RDEF;
        vector< map<string, string> >* SCCP;
        vector< map<string, string> > NCC;
};
#endif
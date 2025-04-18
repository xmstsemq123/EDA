#ifndef _DEF2GNU_H_
#define _DEF2GNU_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "ReadDEF.h"
using namespace std;
class DEF2Gnu{
    public:
        DEF2Gnu(ReadDEF* RDEF, vector< map<string, string> >* SCC);
        void draw();
    private:
        ReadDEF* RDEF;
        vector< map<string, string> >* SCC;
};
#endif
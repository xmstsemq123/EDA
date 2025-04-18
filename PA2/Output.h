#ifndef _OUTPUT_H_
#define _OUTPUT_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "ReadDEF.h"
class Output{
    public:
        Output(ReadDEF* RDEF, vector< map<string, string> >* SCCP, string& OutputFile);
        void StartOutput();
    private:
        ReadDEF* RDEF;
        vector< map<string, string> >* SCCP;
        string OutputFile;
};
#endif
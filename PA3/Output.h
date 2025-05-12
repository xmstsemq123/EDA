#ifndef OUTPUT_H_
#define OUTPUT_H_
#include "ReadFile.h"
#include <deque>
#include <vector>
#include <string>
using namespace std;
using net = ReadFile::net;
using p = ReadFile::p;
class Output{
    public:
        Output(deque<net>* ncP_, string outputFile_);
        void execute();
    private:

    private:
        deque<net>* ncP;
        string outputFile;
};
#endif
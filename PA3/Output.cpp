#include <iostream>
#include <deque>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "Output.h"
#include "ReadFile.h"
using namespace std;
using net = ReadFile::net;
using p = ReadFile::p;

Output::Output(deque<net>* ncP_, string outputFile_)
    :ncP(ncP_), outputFile(outputFile_) {}

void Output::execute(){
    ofstream fout(outputFile);
    if(fout.is_open()){
        for(const auto& nc : *ncP){
            fout << nc.net_id << " " << nc.path.size() - 1 << "\n";
            for(const auto& ep : nc.path){
                if(ep == nc.start){
                    fout << ep.first << " " << ep.second << " ";
                }else if(ep == nc.target){
                    fout << ep.first << " " << ep.second << "\n";
                }else{
                    fout << ep.first << " " << ep.second << "\n" << ep.first << " " << ep.second << " ";
                }
            }
        }
        cout << "Output-file is done!" << endl;
    }else if(fout.fail() || fout.bad()){
        cout << "Occurs some errors when making output file!" << endl;
        exit(1);
    }
    fout.close();
}
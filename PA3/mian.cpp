#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <deque>
#include <algorithm>
#include "ReadFile.h"
#include "AStarAlgorithm.h"
#include "Graph.h"
#include "Output.h"
using namespace std;
using p = ReadFile::p;
using net = ReadFile::net;
int main(int argc, char **argv){
    // check command-line parameter
    if(argc != 3) {
        cout << "Please input correct amount of command-line parameters!" << endl;
        exit(1);
    }
    string inputFile = argv[1], outputFile = argv[2];
    // open input file
    ifstream fin(inputFile);
    stringstream ss;
    ReadFile RF;
    string inData, temp;
    vector<string> IDC;
    // read file and transform to nc
    if(fin.is_open()){
        while(getline(fin, inData)){
            ss << inData;
            while(ss >> temp){
                IDC.push_back(temp);
            }
            RF.readFromEachLine(IDC);
            IDC.clear();
            ss.str("");
            ss.clear();
        }
    }else if(fin.bad() || fin.fail()){
        cout << "Input-file does not exist, or cannot be read!";
        exit(1);
    }
    // RF.showNC();
    fin.close();

    AStarAlgorithm ASA(RF.getGrid(), RF.getLossP());

    deque<net>* ncP = RF.getNCP();
    sort(ncP->begin(), ncP->end(), [&](net& na, net& nb){
        return ASA.heuristic(na.start.first, na.start.second, na.target.first, na.target.second) > ASA.heuristic(nb.start.first, nb.start.second, nb.target.first, nb.target.second);
    });

    
    for(auto& en : *ncP){
        en.path = ASA.execute(en.start, en.target);
        ASA.addBlock(en.path);
        // ASA.addBlock(en.path);
        cout << en.net_id << ": from (" << en.start.first << "," 
            << en.start.second << ") to (" << en.target.first << "," 
            << en.target.second << ")" << endl;
        cout << "path: ";
        for(const auto& ep : en.path){
            cout << "(" << ep.first << "," << ep.second << ") -> ";
        }
        cout << endl;
    }

    Graph genG(ncP);
    genG.draw();

    Output op(ncP, outputFile);
    op.execute();

    return 0;
}
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <sstream>
#include "ReadDEF.h"
#include "Algorithm1.h"
#include "Algorithm2.h"
#include "Output.h"
#include "DEF2Gnu.h"
#include "Displacement.h"
using namespace std;
int main(int argc, char** argv){
    // check if command-line parameters is corrects
    if(argc != 5) {
        cout << "Please input correct amount of command-line parameters!" << endl;
        exit(1);
    }
    // read command-line parameters and assign to variables
    string cellWidth(argv[1]);
    string alpha(argv[2]);
    string inputFile(argv[3]);
    string outputFile(argv[4]);
    // read input file and store into ReadDEF
    // --initialize
    stringstream ss;
    ifstream fin;
    ReadDEF RDEF(cellWidth, alpha);
    // --open input file
    fin.open(inputFile);
    // --get each line from input file and let ReadDEF read it
    string defData;
    vector<string> defDataPara;
    string temp;
    if(fin.is_open()){
        // ----defData = "DESIGN Attack_on_Titan ;"
        while(std::getline(fin, defData)){
            // ----defDataPara = ["DESIGN", "Attack_on_Titan", ";"]
            ss << defData;
            while(ss >> temp){
                defDataPara.push_back(temp);
            }
            // ----let ReadDEF read it
            RDEF.readLine(defDataPara);
            // ----clear all string containers
            defDataPara.clear();
            ss.str("");
            ss.clear();
        }
    }
    // ----Exception proccess 
    else if (fin.bad() || fin.fail()){
        cout << "Reading input files occurs error!" << endl;
        exit(1);
    }
    // close input file stream
    fin.close();
    // sort
    Algorithm2 Alg2(&RDEF);
    Alg2.Sort();
    // output the file
    Output op(&RDEF, Alg2.getSCCP(), outputFile);
    op.StartOutput();
    // draw on gnuplot
    DEF2Gnu DG(&RDEF, Alg2.getSCCP());
    DG.draw();
    // show displacement
    Displacement Dis(&RDEF, Alg2.getSCCP());
    Dis.show();
    return 0;
}
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include "DEF2Gnu.h"
using namespace std;
int main(int argc, char** argv){
    //Check if numbers of Command-line parameters matched.
    if (argc != 5) {
        cout << "Command-line parameters are not correct!" << endl;
        return 0;
    }

    //Assign each parameter from command-line parameters 
    int MSBCSWidth, MSBCSHeight;
    string InputFileSrc, OutputFileSrc;
    stringstream ss;
    ss << argv[1];
    ss >> MSBCSWidth;
    ss.str("");
    ss.clear();
    ss << argv[2];
    ss >> MSBCSHeight;
    ss.str("");
    ss.clear();
    ss << argv[3];
    ss >> InputFileSrc;
    ss.str("");
    ss.clear();
    ss << argv[4];
    ss >> OutputFileSrc;
    ss.str("");
    ss.clear();

    //open the input file (DEF)
    ifstream fin;
    string def_data;
    vector<string> defDataLineArr;
    DEF2Gnu dg(MSBCSWidth, MSBCSHeight, InputFileSrc);
    fin.open(InputFileSrc);
    // read DEF file and transform it
    if (fin.is_open()) {
        while (std::getline(fin, def_data)){
            // to split command-line into parameters
            vector<string> defDataPara;
            string temp;
            // make "- Transistor0 MSBCS" to ["-", "Transistor0", "MSBCS"]
            ss << def_data;
            while (ss >> temp) {
                defDataPara.push_back(temp);
            }
            // transform DEF to GnuPlot-command
            dg.TransEachLine(defDataPara);
            // reset stringstream object
            ss.str("");
            ss.clear();
            // reset vector defDataPara
            defDataPara.clear();
        }
    //Exception Process
    } else if (fin.bad() || fin.fail()){
        cout << "Reading input files occurs error!" << endl;
        exit(1);
    }

    // close stringstream
    fin.close();

    // after transformation is done
    // put gnuCommands into output file
    ofstream outputFile(OutputFileSrc);
    if(!outputFile){
        cout << "Cannot open output file!" << endl;
    }else if(!dg.is_done()){
        cout << "Transform is not done yet!" << endl;
    }else{
        vector<string> gnuCommand = dg.getGnuCommands();
        for (auto data : gnuCommand){
            outputFile << data << "\n";
        }
    }
    outputFile.close();

    return 0;
}
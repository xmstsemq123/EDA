#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <map>
#include <vector>
#include "ReadDEF.h"
using namespace std;

ReadDEF::ReadDEF(int cellWidth, double alpha){
    this->cellWidth = cellWidth;
    this->alpha = alpha;
}
ReadDEF::ReadDEF(string cellWidth, string alpha){
    this->cellWidth = stoi(cellWidth);
    this->alpha = stod(alpha);
}

void ReadDEF::readLine(vector<string>& defDataPara){
    if(defDataPara.size() != 0){
        dpf = defDataPara.front();
        if(dpf == "-"){
            string temp;
            bool recording = false;
            int placedIndex = 0;
            for(const auto& item : defDataPara){
                ++ placedIndex;
                if(item == "-"){
                    recording = true;
                    continue;
                }
                if(item == "+") break;
                if(recording) temp += item + " ";
            }
            Component["Name"] = temp;
            Component["StartX"] = defDataPara[placedIndex+2];
            Component["StartY"] = defDataPara[placedIndex+3];
            Component["Orientation"] = defDataPara[placedIndex+5];
            ComponentContainer.push_back(Component);
        }else if (dpf == "ROW"){
            Row["RowName"] = defDataPara[1];
            Row["SiteName"] = defDataPara[2];
            Row["StartX"] = defDataPara[3];
            Row["StartY"] = defDataPara[4];
            Row["Orientation"] = defDataPara[5];
            Row["HorizontalSite"] = defDataPara[7];
            Row["VerticalSite"] = defDataPara[9];
            Row["XSpace"] = defDataPara[11];
            Row["YSpace"] = defDataPara[12];
            RowContainer.push_back(Row);
        }else if (dpf == "COMPONENTS"){
            ComponentAmount = stoi(defDataPara[1]);
        }else if (dpf == "DESIGN"){
            DesignName = defDataPara[1];
        }else if(dpf == "UNITS"){
            UnitsDistanceMicrons = defDataPara[3];
        }else if(dpf == "DIEAREA"){
            Die["StartX"] = stoi(defDataPara[2]);
            Die["StartY"] = stoi(defDataPara[3]);
            Die["EndX"] = stoi(defDataPara[6]);
            Die["EndY"] = stoi(defDataPara[7]);
        }else if(dpf == "VERSION"){
            DEFVersion = defDataPara[1];
        }else if(dpf == "DIVIDERCHAR"){
            DividerChar = defDataPara[1];
        }else if(dpf == "BUSBITCHARS"){
            BusbitChars = defDataPara[1];
        }
    }
}

void ReadDEF::show(){
    cout << "----------Die----------" << endl;
    for (const auto& d : Die){
        cout << d.first << ": " << d.second << endl;
    }
    cout << endl;
    cout << "----------Rows----------" << endl;
    for (const auto& rows : RowContainer){
        for (const auto& s : rows){
            cout << s.first << ": " << s.second << endl;
        }
        cout << endl;
    }
    cout << "----------Components----------" << endl;
    for (const auto& components : ComponentContainer){
        for (const auto& c : components){
            cout << c.first << ": " << c.second << endl;
        }
        cout << endl;
    }
}
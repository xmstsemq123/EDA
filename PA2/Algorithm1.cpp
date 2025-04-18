#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <cmath>
#include "Algorithm1.h"
using namespace std;

Algorithm1::Algorithm1(ReadDEF *RDEFPointer){
    RDEF = RDEFPointer;
    SCC.resize(RDEF->ComponentContainer.size());
    copy(RDEF->ComponentContainer.begin(), RDEF->ComponentContainer.end(), SCC.begin());
}

void Algorithm1::Sort(){
    // Sort cells according to x-position
    SortByX();
    // main code
    for(int i=0; i<SCC.size(); i++){
        int BestCost = numeric_limits<int>::max();
        int BestRow = 0;
        string originalX = SCC[i]["StartX"];
        string originalY = SCC[i]["StartY"];
        for(int k=0; k<(RDEF->RowContainer).size(); k++){
            int cost = 0;
            cost = PlaceRow(SCC[i], RDEF->RowContainer[k], i);
            if(cost < BestCost){
                BestCost = cost;
                BestRow = k;
            }
            SCC[i]["StartX"] = originalX;
            SCC[i]["StartY"] = originalY;
        }
        cout << SCC[i]["Name"] << ", Best Cost: " << BestCost << ", BestRow: " << BestRow << endl;
        PlaceRow(SCC[i], RDEF->RowContainer[BestRow], i);
        PlacedSite[SCC[i]["StartX"]].push_back(RDEF->RowContainer[BestRow]["StartY"]);
    }
    for(auto a : PlacedSite){
        for(auto b : a.second){
            cout << a.first << ": " << b << endl;
        }
    }
}

int Algorithm1::PlaceRow(map<string, string>& cell, map<string, string>& row, int cellIndex){
    int originalX = stoi(cell["StartX"]);
    int originalY = stoi(cell["StartY"]);
    // insert cell i into row by moving vertically the cell to it
    cell["StartY"] = row["StartY"];
    // calculate the new best x-position of X1
    int theNearestSiteX;
    int testX = originalX;
    theNearestSiteX = FindNearestSiteX(testX, row);
    //check if overlap
    bool is_overlap = false;
    int overlapPointXAmount = RDEF->cellWidth / stoi(row["XSpace"]);
    int checkStartPoint = theNearestSiteX - overlapPointXAmount * stoi(row["XSpace"]);
    int OverlapX;
    for(int i=0; i<overlapPointXAmount*2; i++){
        checkStartPoint += i * stoi(row["XSpace"]);
        if(PlacedSite[to_string(checkStartPoint)].empty()) continue;
        else{
            for(auto& ER : PlacedSite[to_string(checkStartPoint)]){
                if(ER == row["StartY"]){
                    is_overlap = true;
                    OverlapX = checkStartPoint;
                    break;
                }
            }
        }
    }
    if(is_overlap){
        theNearestSiteX = OverlapX + RDEF->cellWidth;
    }
    cell["StartX"] = to_string(theNearestSiteX);
    int cost = abs(theNearestSiteX) + abs(originalY - stoi(cell["StartY"]));
    return cost;
}

int Algorithm1::FindNearestSiteX(int& OX, map<string, string>& row){
    if(OX <= RDEF->Die["StartX"]){
        return RDEF->Die["StartX"];
    }else if(OX >= RDEF->Die["EndX"]){
        return RDEF->Die["EndX"] - RDEF->cellWidth;
    }
    int RowXSpaceInt = stoi(row["XSpace"]);
    int multiSite = OX/RowXSpaceInt;
    int diffSite = OX%RowXSpaceInt;
    int preSite = multiSite * 200;
    int nextSite = preSite + 200;
    int preSiteDiff = OX - preSite;
    int nextSiteDiff = nextSite - OX;
    if(preSiteDiff <= nextSiteDiff){
        return preSite + stoi(row["StartX"]);
    }else return nextSite + stoi(row["StartX"]);
}

// Sort cells according to x-position
void Algorithm1::SortByX(){
    sort(SCC.begin(), SCC.end(), [](map<string, string> &a, map<string, string> &b){
        return stoi(a.at("StartX")) < stoi(b.at("StartX"));
    });
}

void Algorithm1::showSCC(){
    cout << "----------SCC----------" << endl;
    for (const auto& components : SCC){
        for (const auto& c : components){
            cout << c.first << ": " << c.second << endl;
        }
        cout << endl;
    }
}

vector< map<string, string> > *Algorithm1::getSCCP(){
    return &SCC;
}
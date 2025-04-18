#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <cmath>
#include <deque>
#include <chrono>
#include "Algorithm2.h"
using namespace std;

Algorithm2::Algorithm2(ReadDEF* RDEF){
    this->RDEF = RDEF;
    SCC.resize(RDEF->ComponentContainer.size());
    copy(RDEF->ComponentContainer.begin(), RDEF->ComponentContainer.end(), SCC.begin());
}

void Algorithm2::Sort(){
    cout << "The Program Starts." << endl;
    auto start = chrono::high_resolution_clock::now();
    RowAmount = RDEF->RowContainer.size();
    RowSite = stoi(RDEF->RowContainer[0]["HorizontalSite"]);
    SiteYSpace = (RDEF->Die["EndY"] - RDEF->Die["StartY"]) / RowAmount;
    SiteXSpace = (RDEF->Die["EndX"] - RDEF->Die["StartX"]) / RowSite;
    oneCellSite = RDEF->cellWidth / SiteXSpace;
    oneCellSite = (RDEF->cellWidth % SiteXSpace) ? oneCellSite + 1 : oneCellSite;
    RowCellCapacity = RowSite / oneCellSite;
    SortByX();
    SortIntoBoundX();
    SortIntoBoundY();
    PlaceIntoNearestY();
    // AverageMethod();
    // AligntoSiteX();
    PushBackMethod();
    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Program Execution Time: " << duration.count() << " ms" << endl;
    checkLegalization();
}

void Algorithm2::SortByX(){
    sort(SCC.begin(), SCC.end(), [](map<string, string> &a, map<string, string> &b){
        return stoi(a.at("StartX")) < stoi(b.at("StartX"));
    });
}

void Algorithm2::SortIntoBoundX(){
    for(auto& EC : SCC){
        if(stoi(EC["StartX"]) < RDEF->Die["StartX"]){
            EC["StartX"] = to_string(RDEF->Die["StartX"]);
        }else if(stoi(EC["StartX"]) > RDEF->Die["EndX"] - RDEF->cellWidth){
            EC["StartX"] = to_string(RDEF->Die["EndX"] - RDEF->cellWidth);
        }
    }
}

void Algorithm2::SortIntoBoundY(){
    for(auto& EC : SCC){
        if(stoi(EC["StartY"]) < RDEF->Die["StartY"]){
            EC["StartY"] = to_string(RDEF->Die["StartY"]);
        }else if(stoi(EC["StartY"]) > RDEF->Die["EndY"] - SiteYSpace){
            EC["StartY"] = to_string(RDEF->Die["EndY"] - SiteYSpace);
        }else continue;
    }
}

void Algorithm2::PlaceIntoNearestY(){
    int oY, mul, rem, preDiff, nextDiff;
    int ysp = RDEF->Die["StartY"];
    int ECIndex = 0;
    for(auto& EC : SCC){
        oY = stoi(EC["StartY"]);
        rem = oY % SiteYSpace;
        if(rem == 0) continue;
        mul = oY / SiteYSpace;
        if(mul == RowAmount - 1){
            EC["StartY"] = to_string(ysp + SiteYSpace * (RowAmount - 1));
            continue;
        }
        preDiff = rem;
        nextDiff = SiteYSpace - preDiff;
        if(preDiff <= nextDiff){
            EC["StartY"] = to_string(ysp + mul * SiteYSpace);
        }else if(preDiff > nextDiff){
            EC["StartY"] = to_string(ysp + (mul + 1) * SiteYSpace);
        }
    }
}

void Algorithm2::AverageMethod(){
    for(int i=0; i<SCC.size(); i++){
        RowPlaced.resize(RDEF->RowContainer.size());
        RowPlaced[stoi(SCC[i]["StartY"]) / SiteYSpace].push_back(i);
    }
    deque<int> ExRowCellIndex;
    for(int i=0; i<RowPlaced.size(); i++){
        if(RowPlaced[i].size() > RowCellCapacity){
            int ExCellDiff = RowPlaced[i].size() - RowCellCapacity;
            for(int k=0; k<ExCellDiff; k++){
                ExRowCellIndex.push_back(RowPlaced[i].at(RowCellCapacity));
                RowPlaced[i].erase(RowPlaced[i].begin() + RowCellCapacity);
            }
        }
    }
    int startRow = 0;
    while(ExRowCellIndex.size() != 0){
        if(startRow >= RowPlaced.size()) break;
        while(RowPlaced[startRow].size() < RowCellCapacity){
            SCC[ExRowCellIndex.front()]["StartY"] = to_string(startRow * SiteYSpace);
            RowPlaced[startRow].push_back(ExRowCellIndex.front());
            ExRowCellIndex.pop_front();
            if(ExRowCellIndex.size() == 0) break;
        }
        ++startRow;
    }
    for(int i=0; i<RowPlaced.size(); i++){
        if(RowPlaced[i].size() == 0) continue;
        int sxp = RDEF->Die["StartX"];
        int remainSpace = (RDEF->Die["EndX"] - sxp - RowPlaced[i].size() * oneCellSite * SiteXSpace);
        remainSpace /= SiteXSpace;
        for(int k=0; k<RowPlaced[i].size(); k++){
            SCC[RowPlaced[i][k]]["StartX"] = to_string(sxp);
            sxp += oneCellSite * SiteXSpace;
        }
        for(int k=0; k<remainSpace; k++){
            if(RowPlaced[i].size() <= 1) break;
            int csi = k % RowPlaced[i].size() + 1;
            for(int sxi=csi; sxi<RowPlaced[i].size(); sxi++){
                SCC[RowPlaced[i][sxi]]["StartX"] = to_string(stoi(SCC[RowPlaced[i][sxi]]["StartX"]) + SiteXSpace);
            }
        }
    }
}

void Algorithm2::AligntoSiteX(){
    int oX, mul, rem, preDiff, nextDiff;
    int xsp = RDEF->Die["StartX"];
    for(auto& EC : SCC){
        oX = stoi(EC["StartX"]);
        rem = oX % SiteXSpace;
        if(rem == 0) continue;
        mul = oX / SiteXSpace;
        if(mul == RowSite - 1){
            EC["StartX"] = to_string(xsp + SiteXSpace * (RowSite - 1));
            continue;
        }
        preDiff = rem;
        nextDiff = SiteXSpace - preDiff;
        if(preDiff <= nextDiff){
            EC["StartX"] = to_string(xsp + mul * SiteXSpace);
        }else if(preDiff > nextDiff){
            EC["StartX"] = to_string(xsp + (mul + 1) * SiteXSpace);
        }
    }
    SortByX();
    for(int i=0; i<SCC.size(); i++){
        RowPlaced.resize(RDEF->RowContainer.size());
        int rowIndex = stoi(SCC[i]["StartY"]) / SiteYSpace;
        RowPlaced[rowIndex].push_back(i);
    }
    overlapCells.resize(RowPlaced.size());
    for(int i=0; i<RowPlaced.size(); i++){ 
        if(RowPlaced[i].size() < 1) continue;
        for(int j=RowPlaced[i].size() - 1; j>=0; j--){
            if(is_overlap(i, j)){
                // cout << "----*overlap: " << SCC[RowPlaced[i][j]]["Name"] << endl;
                overlapCells[i].push_back(RowPlaced[i][j]);
                RowPlaced[i].erase(RowPlaced[i].begin() + j);
            }
        }
    }
    for(int i=0; i<RowPlaced.size(); i++){
        for(int j=0; j<RowPlaced[i].size(); j++){
            RPSX[i][stoi(SCC[RowPlaced[i][j]]["StartX"])] = true;
        }
    }
    // for(auto& ER : RPSX){
    //     cout << "-----" << ER.first << "-----" << endl;
    //     for(auto& EC : ER.second){
    //         cout << EC.first << endl;
    //     }
    // }
}

void Algorithm2::PushBackMethod(){
    AligntoSiteX();
    int oneCellXSpace = oneCellSite * SiteXSpace;
    for(int row=0; row<overlapCells.size(); row++){
        for(int com=0; com<overlapCells[row].size(); com++){
            int orow = row; // original row
            int crow = orow; // current row
            int nrow = orow; // next row-
            int prow = orow; // previous row
            int changeRowTimes = 0;
            int ox = stoi(SCC[overlapCells[row][com]]["StartX"]);
            for(int rowAmount=0; rowAmount<RDEF->RowContainer.size(); rowAmount++){
                // determine which row the cell try to put into
                if(changeRowTimes % 2 != 0){ // in next row
                    if(nrow > RDEF->RowContainer.size() - 1) continue;
                }else{ // in pre row
                    if(prow < 0) continue;
                }
                // try to put cell into the row
                int times = 0;
                int rs = ox , ls = ox;
                bool found_space = true;
                while(is_overlap_sitex(crow, stoi(SCC[overlapCells[row][com]]["StartX"]))){
                    // cout << SCC[overlapCells[row][com]]["Name"] << " overlap!" << endl;
                    ++times;
                    bool overEnd = rs > RDEF->Die["EndX"];
                    bool overStart = ls < RDEF->Die["StartX"];
                    if(overEnd && overStart){
                        SCC[overlapCells[row][com]]["StartX"] = to_string(ox);
                        found_space = false;
                        break;
                    }
                    // cout << SCC[overlapCells[row][com]]["Name"] << ": " << ls << "," << rs << " " << RDEF->Die["EndX"] - oneCellXSpace << " " << times << " " << crow << endl;
                    if(times%2 != 0){
                        rs += SiteXSpace;
                        if(rs > RDEF->Die["EndX"] - oneCellXSpace) continue;
                        SCC[overlapCells[row][com]]["StartX"] = to_string(rs);
                    }else{
                        ls -= SiteXSpace;
                        if(ls < RDEF->Die["StartX"]) continue;
                        SCC[overlapCells[row][com]]["StartX"] = to_string(ls);
                    }
                }
                // orow = crow; 
                if(!found_space){
                    ++changeRowTimes;
                    if(changeRowTimes % 2 != 0){ // in next row
                        if(!(nrow >= RDEF->RowContainer.size() - 1)){
                            ++nrow;
                            crow = nrow;
                        }
                    }else{ // in pre row
                        if(!(prow < 0)){
                            --prow;
                            crow = prow;
                        }
                    }
                    SCC[overlapCells[row][com]]["StartX"] = to_string(ox);
                    SCC[overlapCells[row][com]]["StartY"] = to_string(crow * SiteYSpace);
                }else{
                    int spi = 0;
                    for(int fi=0; fi<RowPlaced[crow].size(); fi++){
                        spi = fi;
                        if(stoi(SCC[overlapCells[row][com]]["StartX"]) > stoi(SCC[RowPlaced[crow][fi]]["StartX"])) break;
                    }
                    // cout << crow  << " " << spi<< endl;
                    RowPlaced[crow].insert(RowPlaced[crow].begin() + spi, overlapCells[row][com]);
                    // cout << "n" << endl;
                    RPSX[crow][stoi(SCC[overlapCells[row][com]]["StartX"])] = true;
                    break;
                }
            }
        }
    }
    // for(auto& ER : RPSX){
    //     cout << "-----" << ER.first << "-----" << endl;
    //     for(auto& EC : ER.second){
    //         cout << EC.first << ": " << EC.second << endl;
    //     }
    // }
}

bool Algorithm2::is_overlap_sitex(int RowIndex, int SiteX){
    int oneOverlapXSpace = (oneCellSite - 1) * SiteXSpace;
    int startSiteX = SiteX - oneOverlapXSpace;
    int endSiteX = SiteX + oneOverlapXSpace;
    bool is_overlap = false;
    for(int currentSiteX=startSiteX; currentSiteX<=endSiteX; currentSiteX += SiteXSpace){
        if(currentSiteX < RDEF->Die["StartX"] || currentSiteX > RDEF->Die["EndX"]) continue;
        // cout << "currentSiteX: " << currentSiteX << ", RPSX: " << RPSX[RowIndex][currentSiteX] << endl;
        if(RPSX[RowIndex][currentSiteX] == true){
            is_overlap = true;
            break;
        }
    }
    // if(!is_overlap) cout << "not overlap !" << endl;
    return is_overlap;
}

bool Algorithm2::is_overlap(int RowIndex, int hereSCCIndex){
    int hereX = stoi(SCC[RowPlaced[RowIndex][hereSCCIndex]]["StartX"]);
    int cellWidth = RDEF->cellWidth;
    bool is_overlap = false;
    if(RowPlaced[RowIndex].size() > 1){
        // cout << "nowX: " << hereX << " " << SCC[RowPlaced[RowIndex][hereSCCIndex]]["Name"] << endl;
        if(hereSCCIndex != 0){
            int preX = stoi(SCC[RowPlaced[RowIndex][hereSCCIndex - 1]]["StartX"]);
            // cout << "preX: " << preX << " " << SCC[RowPlaced[RowIndex][hereSCCIndex - 1]]["Name"] << endl;
            if((hereX > preX - cellWidth) && (hereX < preX + cellWidth)) is_overlap = true;
        }
        if(hereSCCIndex <= RowPlaced[RowIndex].size() - 2){
            int nextX = stoi(SCC[RowPlaced[RowIndex][hereSCCIndex + 1]]["StartX"]);
            // cout << "nextX: " << nextX << " " << SCC[RowPlaced[RowIndex][hereSCCIndex + 1]]["Name"] << endl;
            if((hereX > nextX - cellWidth) && (hereX < nextX + cellWidth)) is_overlap = true;
        }
    } 
    return is_overlap;
}

void Algorithm2::checkLegalization(){
    // check in bound
    int overXBound = 0;
    int overYBound = 0;
    int notOnRow = 0;
    int notInSite = 0;
    int overlap = 0;
    for(auto& EC : SCC){
        if((stoi(EC["StartX"]) < RDEF->Die["StartX"]) || (stoi(EC["StartX"]) > RDEF->Die["EndX"])){
            ++overXBound;
        }
        if((stoi(EC["StartY"]) < RDEF->Die["StartY"]) || (stoi(EC["StartY"]) > RDEF->Die["EndY"])){
            ++overYBound;
        }
        if(stoi(EC["StartY"]) % SiteYSpace != 0){
            ++notOnRow;
        }
        if(stoi(EC["StartX"]) % SiteXSpace != 0){
            ++notInSite;
        }
    }
    for(auto& ECC : RowPlaced){
        sort(ECC.begin(), ECC.end(), [&](int &a, int &b){
            return stoi(SCC[a]["StartX"]) < stoi(SCC[b]["StartX"]);
        });
    }
    int cellWidth = RDEF->cellWidth;
    for(auto& ERCC : RowPlaced){
        if(ERCC.size() < 2) continue;
        for(int i=0; i<ERCC.size(); i++){
            bool is_overlap = false;
            int ci = ERCC[i];
            int herex = stoi(SCC[ci]["StartX"]);
            if(i <= ERCC.size()-2){
                int nextx = stoi(SCC[ERCC[i+1]]["StartX"]);
                if((nextx > herex - cellWidth) && (nextx < herex + cellWidth)) is_overlap = true;
            }
            if(i != 0){
                int prex = stoi(SCC[ERCC[i-1]]["StartX"]);
                if((prex > herex - cellWidth) && (prex < herex + cellWidth)) is_overlap = true;
            }
            if(is_overlap) ++overlap;
        }
    }
    if(overXBound == 0) cout << "1.(O) 沒有cell超出X界線" << endl;
    else cout << "1.(X) 有" << overXBound << "個cell超出X界線" << endl;
    if(overYBound == 0) cout << "2.(O) 沒有cell超出Y界線" << endl;
    else cout << "2.(X) 有" << overYBound << "個cell超出Y界線" << endl;
    if(notOnRow == 0) cout << "3.(O) cell全在Row上" << endl;
    else cout << "3.(X) 有" << notOnRow << "個cell不在Row上" << endl;
    if(notInSite == 0) cout << "4.(O) cell全在Site上" << endl;
    else cout << "4.(X) 有" << notInSite << "個cell不在Site上" << endl;
    if(overlap == 0) cout << "4.(O) cell完全沒有overlap" << endl;
    else cout << "5.(X) 有" << overlap << "個cell有overlap" << endl;
}

vector<map<string, string> >* Algorithm2::getSCCP(){
    return &SCC;
}
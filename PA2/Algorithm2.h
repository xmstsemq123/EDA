#ifndef _ALGORITHM2_H_
#define _ALGORITHM2_H_
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "ReadDEF.h"
using namespace std;
class Algorithm2{
    public:
        Algorithm2(ReadDEF* RDEF);
        vector<map<string, string> >* getSCCP();
        void Sort();
        void checkLegalization();
    private:
        void SortByX();
        void SortIntoBoundX();
        void SortIntoBoundY();
        void PlaceIntoNearestY();
        void AligntoSiteX();
        void PushBackMethod();
        void AverageMethod();
        bool is_overlap(int RowIndex, int hereSCCIndex);
        bool is_overlap_sitex(int RowIndex, int SiteX);
    private:
        ReadDEF* RDEF;
        int OverlapCellX;
        int SiteYSpace;
        int RowAmount;
        int RowSite;
        int SiteXSpace;
        int oneCellSite;
        int RowCellCapacity;

        map< int, map< int, bool > > RPSX;
        // RowPlacedSiteX = {
        //     RowIndex1: {
        //         SiteX1: true,
        //         SiteX2: ture, ...
        //     },
        //     RowIndex2: ...
        // }
        deque< deque<int> > overlapCells; // [Row][Component index in SCC]
        vector< map<string, string> > SCC;
        vector< vector<int> > RowPlaced; // [Row][Component index in SCC]
};
#endif
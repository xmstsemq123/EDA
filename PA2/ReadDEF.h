#ifndef _READDEF_H_
#define _READDEF_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
class ReadDEF{
    public:
        friend class Algorithm1;
        friend class Algorithm2;
        friend class Output;
        friend class DEF2Gnu;
        friend class Displacement;
    // for functions
    public:
        // constructors for two types of parameters
        ReadDEF();
        ReadDEF(int cellWidth, double alpha);
        ReadDEF(string cellWidth, string alpha);
        // read DEF each line and store information after transforming
        void readLine(vector<string>& defDataPara);
        void show();
    // for variables
    private:
        // variables for readLine()
        string dpf; // first element of defDataPara
        // placement parameter
        double alpha;
        // DEF initialization settings
        string DEFVersion;
        string DividerChar;
        string BusbitChars;
        string DesignName;
        string UnitsDistanceMicrons;
        // die information
        map<string, int> Die {
            {"StartX", 0},
            {"StartY", 0},
            {"EndX", 0},
            {"EndY", 0}
        };
        // set of row
        map<string, string> Row {
            {"RowName", ""},
            {"SiteName", ""},
            {"StartX", ""},
            {"StartY", ""},
            {"Orientation", ""},
            {"HorizontalSite", ""},
            {"VerticalSite", ""},
            {"XSpace", ""},
            {"YSpace", ""}
        };
        vector< map<string, string> > RowContainer;
        // set of components
        int ComponentAmount;
        int cellWidth;
        map<string, string> Component {
            {"Name", ""},
            {"StartX", ""},
            {"StartY", ""},
            {"Orientation", ""}
        };
        vector< map<string, string> > ComponentContainer;
};
#endif
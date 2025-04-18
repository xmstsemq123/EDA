#include <iostream>
#include <vector>
#include <string>
using namespace std;
class DEF2Gnu {
public:
    DEF2Gnu (int MSBCSWidth, int MSBCSHeight, string DEFFileName);
    void TransEachLine(vector<string>& defDataLine);
    vector<string> getGnuCommands() const;
    bool is_done() const;
    void set_def_file_name(string& name);
private:
    void FinalGnuSetting();
    // flags
        // implement type
    bool DESIGN_FINISH;
    bool INITIAL_SETTING; 
    bool COMPONENTS_SETTING;
    bool SPECIALNETS_SETTING;
        // for each component block
    bool COMPONENT_WHOLE_BLOCK_FINISH;
        // for each net wire
    bool NET_WHOLE_WIRE_FINISH;
        // check if tansform is done
    bool WHOLE_TRANSFORM_IS_DONE;
        // check if transformation occurs error
    bool TRANSFORM_ERROR;

    // global data & diearea data
    string DEFFileName;
    string units_distance_microns; // `units_distance_microns` units = 1 um
    string diearea_coordinate[2]; // [x, y]
    string diearea_size[2]; // [width, height]
    string gnuInitialSetting;
    string gnuFinalSetting;

    //MSBCS information
    int MSBCSWidth;
    int MSBCSHeight;

    // each component information
    vector<string> Component; // [name, type, x, y]

    // each net information
    vector<string> Net; // [name, layer, wire width, x1, y1, x2, y2, direction]

    // Gnu command
    vector<string> GnuCommand;

    // number of gnu object
    int objectOrderNumber;
};
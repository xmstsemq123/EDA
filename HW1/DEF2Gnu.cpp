#include "DEF2Gnu.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;

// constructor
DEF2Gnu::DEF2Gnu(int MSBCSWidth = 0, int MSBCSHeight = 0, string DEFFileName = ""){
    this->DEFFileName = DEFFileName;
    this->MSBCSWidth = MSBCSWidth;
    this->MSBCSHeight = MSBCSHeight;
    objectOrderNumber = 1;
    DESIGN_FINISH = false;
    INITIAL_SETTING = true;
    COMPONENTS_SETTING = false;
    SPECIALNETS_SETTING = false;
    COMPONENT_WHOLE_BLOCK_FINISH = false;
    NET_WHOLE_WIRE_FINISH = false;
    WHOLE_TRANSFORM_IS_DONE = false;
    TRANSFORM_ERROR = false;
    gnuInitialSetting = "reset\n"
                        "set termoption noenhanced\n";
    GnuCommand.push_back(gnuInitialSetting);
}

// set fianl gnuPlot-command
void DEF2Gnu::FinalGnuSetting(){
    string endX = to_string(stoi(diearea_coordinate[0]) + stoi(diearea_size[0]));
    string endY = to_string(stoi(diearea_coordinate[1]) + stoi(diearea_size[1]));
    gnuFinalSetting = "set xtics 10000\n"
    "set ytics 10000\n"
    "set size square\n"
    "set title \"Die routing graphic from " + DEFFileName +"(Diearea: " + diearea_size[0] + "x" + diearea_size[1] +")\"\n"                  
    "set xlabel \"X\"\n"
    "set ylabel \"Y\"\n"
    "set grid\n"
    "set xrange [" + diearea_coordinate[0] + ":" + endX + "]\n"
    "set yrange [" + diearea_coordinate[1] + ":" + endY + "]\n"
    "plot [" + diearea_coordinate[0] + ":" + endX + "][" + diearea_coordinate[1] + ":" + endY + "] 0\n"
    "set terminal png size 1024,768\n"
    "set output \"output.png\"\n"
    "replot\n"
    "replot\n";
    GnuCommand.push_back(gnuFinalSetting);
    //"set terminal png size 1024,768\n"
    //"set output \"output.png\"\n"
}

// tansform for each command-line
void DEF2Gnu::TransEachLine(vector<string>& defDataPara){
    // check if defDataPara is empty
    if(defDataPara.size() == 0) return;
    try{
        // initial setting of design
        if(INITIAL_SETTING){
            // UNITS DISTANCE MICRONS 1000 ;
            if(defDataPara.front() == "UNITS"){
                units_distance_microns = defDataPara.at(3);
            }
            // DIEAREA ( 0 0 ) ( 82970 80070 ) ;
            else if (defDataPara.front() == "DIEAREA"){
                diearea_coordinate[0] = defDataPara.at(2);
                diearea_coordinate[1] = defDataPara.at(3);
                diearea_size[0] = defDataPara.at(6);
                diearea_size[1] = defDataPara.at(7);
            } 
            // COMPONENTS 64 ;
            else if (defDataPara.front() == "COMPONENTS"){
                INITIAL_SETTING = false;
            }
        }

        // check it is sure entering COMPONENTS
        if(defDataPara.front() == "COMPONENTS" && INITIAL_SETTING == false){
            COMPONENTS_SETTING = true;
        }
        
        // design of components
        if(COMPONENTS_SETTING){
            // - Transistor0 MSBCS
            if(defDataPara.front() == "-"){
                COMPONENT_WHOLE_BLOCK_FINISH = false;
                Component.push_back(defDataPara.at(1)); // name
                Component.push_back(defDataPara.at(2)); // type
            } 
            // + PLACED ( 0 2980 ) N ;
            else if(defDataPara.front() == "+"){
                COMPONENT_WHOLE_BLOCK_FINISH = true;
                Component.push_back(defDataPara.at(3)); // x
                Component.push_back(defDataPara.at(4)); // y
            } 
            // END COMPONENTS
            else if(defDataPara.front() == "END"){
                COMPONENTS_SETTING = false;
            }
            // if describing a component block is done
            if(COMPONENT_WHOLE_BLOCK_FINISH == true){
                // temporally scope to store gnu command
                string gnuCommandString;
                // component name & type
                string ComponentName = Component.at(0);
                string ComponentType = Component.at(1);
                // coordinates of the component
                int startX = stoi(Component.at(2));
                int startY = stoi(Component.at(3));
                int endX = startX + MSBCSWidth;
                int endY = startY + MSBCSHeight;
                int centerX = (startX + endX) / 2;
                int centerY = (startY + endY) / 2;
                // gnu command for generating a rectangular object
                gnuCommandString = "set object " + to_string(objectOrderNumber) + " rect from " + to_string(startX) + "," + to_string(startY) + " to " + to_string(endX) + "," + to_string(endY) + " lw 1 fs solid fc rgb \"#FFD433\"";               
                GnuCommand.push_back(gnuCommandString);
                // gnu command for generating a label onto that object
                gnuCommandString = "set label \"" + ComponentName + "\" at " + to_string(centerX) + "," + to_string(centerY) + " font \"Times, 8px\"" + " center ";               
                GnuCommand.push_back(gnuCommandString);
                // adjustment after finishing a block
                Component.clear();
                objectOrderNumber += 1;
                COMPONENT_WHOLE_BLOCK_FINISH = false;
            }
        }

        // check it is sure entering SPECIALNETS
        if(defDataPara.front() == "SPECIALNETS" && COMPONENTS_SETTING == false){
            SPECIALNETS_SETTING = true;
        }

        // design of nets
        if(SPECIALNETS_SETTING){
            // - Metal3_0
            if(defDataPara.front() == "-"){
                NET_WHOLE_WIRE_FINISH = false;
                Net.push_back(defDataPara.at(1)); // name
            }
            // + ROUTED ME3 440 ( 7630 0 ) ( * 80070 ) ;
            else if(defDataPara.front() == "+"){
                NET_WHOLE_WIRE_FINISH = true;
                Net.push_back(defDataPara.at(2)); // layer
                Net.push_back(defDataPara.at(3)); // wire width
                Net.push_back(defDataPara.at(5)); // x1
                Net.push_back(defDataPara.at(6)); // y1
                string direction = "";
                // check if x2 == * ?
                if(defDataPara.at(9) == "*") {
                    // x2 == x1
                    Net.push_back(defDataPara.at(5));
                    direction = "vertical";
                } 
                else Net.push_back(defDataPara.at(9)); // x2 != x1
                //check if y2 == * ?
                if(defDataPara.at(10) == "*") {
                    // y2 == y1
                    Net.push_back(defDataPara.at(6));
                    direction = "horizontal";
                }
                else Net.push_back(defDataPara.at(10)); // y2 != y1
                // put direction into defDataPara
                Net.push_back(direction);
            }
            // END SPECIALNETS
            else if(defDataPara.front() == "END"){
                SPECIALNETS_SETTING = false;
                WHOLE_TRANSFORM_IS_DONE = true;
            }
            // if describing a net wire is done
            if(NET_WHOLE_WIRE_FINISH == true){
                // temporally scope to store gnu command
                string gnuCommandString;
                // net name & layer
                string netName = Net.at(0);
                string layer = Net.at(1);
                // net wire width and coordinate
                int wireWidth = stoi(Net.at(2));
                int x1 = stoi(Net.at(3));
                int y1 = stoi(Net.at(4));
                int x2 = stoi(Net.at(5));
                int y2 = stoi(Net.at(6));
                int startX, startY, endX, endY, labelX, labelY;
                string direction = Net.at(7);
                if(direction == "vertical"){
                    startX = x1 - wireWidth/2;
                    startY = y1;
                    endX = x2 + wireWidth/2;
                    endY = y2;
                    labelX = x1;
                    labelY = y2 - 6500;
                } else if(direction == "horizontal"){
                    startX = x1;
                    startY = y1 - wireWidth/2;
                    endX = x2;
                    endY = y2 + wireWidth/2;
                    labelX = (x1 + x2)/2;
                    labelY = y1;
                }
                // gnu command for generating a rectangular object
                string ifBehind = (layer == "ME3") ? " behind" : "" ;
                string wireColor = (layer == "ME3") ? "\"#90FF33\"" : "\"#339FFF\"" ; // ME3->green, ME4->blue
                gnuCommandString = "set object " + to_string(objectOrderNumber) + " rect from " + to_string(startX) + "," + to_string(startY) + " to " + to_string(endX) + "," + to_string(endY) + " lw 1 fs solid fc rgb " + wireColor + ifBehind;
                GnuCommand.push_back(gnuCommandString);
                // gnu command for generating a label
                string gnuForDirection = (direction == "vertical") ? " rotate by 90" : " center" ;
                gnuCommandString = "set label \"" + netName + "\" at " + to_string(labelX) + "," + to_string(labelY) + " font \"Times, 6px\"" + gnuForDirection;
                GnuCommand.push_back(gnuCommandString);
                // adjustment after finishing a net
                Net.clear();
                objectOrderNumber += 1;
                NET_WHOLE_WIRE_FINISH = false;
            }
        }

        // check it is sure done
        if(defDataPara.front() == "END" && SPECIALNETS_SETTING == false && defDataPara.size() >= 2){
            if(defDataPara.at(1) == "DESIGN"){
                WHOLE_TRANSFORM_IS_DONE = true;
                FinalGnuSetting();
            }
        }
    }catch(...){
        cout << "Your DEF file sytanx error!" << endl;
        TRANSFORM_ERROR = true;
        exit(1);
    }
}

vector<string> DEF2Gnu::getGnuCommands() const {
    return GnuCommand;
}

bool DEF2Gnu::is_done() const{
    return WHOLE_TRANSFORM_IS_DONE;
}

void DEF2Gnu::set_def_file_name(string& name){
    DEFFileName = name;
}
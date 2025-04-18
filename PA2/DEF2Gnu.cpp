#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "DEF2Gnu.h"
using namespace std;

DEF2Gnu::DEF2Gnu(ReadDEF* RDEF, vector< map<string, string> >* SCC){
    this->RDEF = RDEF;
    this->SCC = SCC;
}

void DEF2Gnu::draw(){
    ofstream fdat("rows.dat");
    fdat << "# center_x\tcenter_y\thalf_width\thalf_height\n";
    int height, width, center_x, center_y;
    for(auto& ER : RDEF->RowContainer){
        height = (RDEF->Die["EndY"]-RDEF->Die["StartY"]) / RDEF->RowContainer.size();
        width = stoi(ER["XSpace"])*stoi(ER["HorizontalSite"]);
        center_x = stoi(ER["StartX"]) + width / 2;
        center_y = stoi(ER["StartY"]) + height / 2;
        fdat << center_x << "\t" << center_y << "\t" << width / 2 << "\t" << height / 2 << "\n";
    }
    fdat.close();

    fdat.open("components.dat");
    fdat << "# center_x\tcenter_y\thalf_width\thalf_height\n";
    width = RDEF->cellWidth;
    for(auto& EC : *SCC){
        center_x = stoi(EC["StartX"]) + width / 2;
        center_y = stoi(EC["StartY"]) + height / 2;
        fdat << center_x << "\t" << center_y << "\t" << width / 2 << "\t" << height / 2 << "\n";
    }
    fdat.close();

    fdat.open("NCC.dat");
    fdat << "# center_x\tcenter_y\thalf_width\thalf_height\n";
    width = RDEF->cellWidth;
    for(auto& EC : RDEF->ComponentContainer){
        center_x = stoi(EC["StartX"]) + width / 2;
        center_y = stoi(EC["StartY"]) + height / 2;
        fdat << center_x << "\t" << center_y << "\t" << width / 2 << "\t" << height / 2 << "\n";
    }
    fdat.close();

    ofstream fgnu("draw_row.gp");
    fgnu << "set title \"DEF ROW Layout\"\n"
            "set xlabel \"X (micron)\"\n"
            "set ylabel \"Y (micron)\"\n"
            "set size ratio -1\n"
            "set xrange [-20000:" << RDEF->Die["EndX"] + 20000 << "]\n"
            "set yrange [-20000:" << RDEF->Die["EndY"] + 20000 << "]\n"
            "set style fill solid 0.3\n"
            "\n"
            "plot \\\n"
            "'rows.dat' using 1:2:3:4 with boxxyerrorbars lc rgb \"light-blue\", \\\n"
            "'NCC.dat' using 1:2:3:4 with boxxyerrorbars lc rgb \"gray\", \\\n"
            "'components.dat' using 1:2:3:4 with boxxyerrorbars lc rgb \"red\"\n"
            "set terminal png size 2048,1024\n"
            "set output \"output.png\"\n"
            "replot";
    fgnu.close();
}
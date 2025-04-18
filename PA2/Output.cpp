#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <fstream>
#include "Output.h"

Output::Output(ReadDEF* RDEF, vector< map<string, string> >* SCCP, string& OutputFile){
    this->RDEF = RDEF;
    this->OutputFile = OutputFile;
    this->SCCP = SCCP;
}

void Output::StartOutput(){
    ofstream fout;
    fout.open(OutputFile);
    // initial setting
    fout << "VERSION " << RDEF->DEFVersion << " ;\n";
    fout << "DIVIDERCHAR " << RDEF->DividerChar << " ;\n";
    fout << "BUSBITCHARS " << RDEF->BusbitChars << " ;\n";
    fout << "DESIGN " << RDEF->DesignName << " ;\n";
    fout << "UNITS DISTANCE MICRONS " << RDEF->UnitsDistanceMicrons << " ;\n";
    fout << "\n";
    // diearea
    fout << "DIEAREA ( " << RDEF->Die["StartX"] << " " << RDEF->Die["StartY"] << " ) ( " << RDEF->Die["EndX"] << " " << RDEF->Die["EndY"] << " ) ;\n";
    fout << "\n";
    // rows
    for(auto& ER : RDEF->RowContainer){
        fout << "ROW " << ER["RowName"] << " " << ER["SiteName"] << " " << ER["StartX"] << " " << ER["StartY"] << " " << ER["Orientation"] << " DO " << ER["HorizontalSite"] << " BY " << ER["VerticalSite"] << " STEP " << ER["XSpace"] << " " << ER["YSpace"] << " ;\n";
    }
    fout << "\n";
    // components
    fout << "COMPONENTS " << RDEF->ComponentAmount << " ;\n";
    for(auto& EC : *SCCP){
        fout << "- " << EC["Name"] << "+ PLACED ( " << EC["StartX"] << " " << EC["StartY"] << " ) " << EC["Orientation"] << " ;\n";  
    }
    fout << "END COMPONENTS\n";
    fout << "\n";
    fout << "END DESIGN";
    fout.close();
}
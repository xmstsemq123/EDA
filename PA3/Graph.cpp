#include <iostream>
#include "Graph.h"
#include "ReadFile.h"
#include <deque>
#include <fstream>
using namespace std;
using net = ReadFile::net;

Graph::Graph(deque<net>* nc_)
    :nc(nc_) {}

void Graph::draw(){
    ofstream fout("./gp/routing_lines.dat");
    for(const auto& en : *nc){
        fout << "# net " << en.net_id << "\n";
        for(const auto& ens : en.path){
            fout << ens.first << " " << ens.second << "\n";
        }
        fout << "\n";
    }
    fout.close();

    fout.open("./gp/plot_routing.gp");
    fout << "set terminal pngcairo size 600,600 enhanced font 'Arial,12'\n"
            "set output 'routing_plot.png'\n"
            "set title \"PIC Routing Result\"\n"
            "set size ratio -1\n"
            "set grid\n"
            "set xlabel \"X\"\n"
            "set ylabel \"Y\"\n"
            "plot for [i=0:2] 'routing_lines.dat' index i using 1:2 with lines lw 2";
    fout.close();
}
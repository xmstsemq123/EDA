#include <iostream>
#include <string>
#include <unordered_map>
#include <deque>
#include <utility>
#include <vector>
#include <cctype>
#include <algorithm>
#include "ReadFile.h"
using namespace std;

ReadFile::ReadFile(){}

ReadFile::net::net(const string& nid, const p& s, const p& t)
    :net_id(nid), start(s), target(t) {}

void ReadFile::readFromEachLine(vector<string> EFS){
    if(EFS.size() == 0) return;
    string head = EFS.at(0);
    if(head == "grid"){
        grid.first = stoi(EFS.at(1));
        grid.second = stoi(EFS.at(2));
    }else if(head == "propagation"){
        loss["propagation"] = stoi(EFS.at(2));
    }else if(head == "crossing"){
        loss["crossing"] = stoi(EFS.at(2));
    }else if(head == "bending"){
        loss["bending"] = stoi(EFS.at(2));
    }else if(head == "num"){
        numNet = stoi(EFS.at(2));
    }else if(EFS.size() == 5){
        auto start = make_pair(stoi(EFS.at(1)), stoi(EFS.at(2)));
        auto target = make_pair(stoi(EFS.at(3)), stoi(EFS.at(4)));
        net rnet(head, start, target);
        nc.push_back(rnet);
    }
}

void ReadFile::showNC() const{
    for(const auto& en : nc){
        cout << en.net_id << ": from (" << en.start.first << "," << en.start.second << ") to (" << en.target.first << "," << en.target.second << ")" << endl;
    }
}

ReadFile::p ReadFile::getGrid(){
    return grid;
}

ReadFile::um* ReadFile::getLossP(){
    return &(this->loss);
}

int ReadFile::getNumNet(){
    return numNet;
}

deque<ReadFile::net>* ReadFile::getNCP(){
    return &(this->nc);
}
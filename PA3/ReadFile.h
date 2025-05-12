#ifndef READFILE_H_
#define READFILE_H_
#include <iostream>
#include <string>
#include <unordered_map>
#include <deque>
#include <utility>
#include <vector>
#include <cctype>
#include <algorithm>
using namespace std;
class ReadFile{
    // namespace definition
    public:
        using p = pair<int, int>;
        using um = unordered_map<string, int>;
        struct net {
            string net_id;
            p start, target; // <Xs, Ys>, <Xt, Yt>
            vector<p> path;
            net(const string& nid, const p& s, const p& t);
        };
    // functions definition
    public:
        ReadFile();
        void readFromEachLine(vector<string> EFS);
        void showNC() const;
        p getGrid();
        um* getLossP();
        int getNumNet();
        deque<net>* getNCP();
    private:

    // variables definition
    private:
        p grid; // <# of hor, # of ver>
        um loss; // propogation, crossing, bending
        int numNet;
        deque<net> nc; // nets container
}; 
#endif
#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include "AStarAlgorithm.h"
using namespace std;
using p = ReadFile::p;
using um = ReadFile::um;
// in struct Node
AStarAlgorithm::Node::Node(int x_, int y_, int g_, int h_, int c_, int b_, string direction_, um* lossP_, Node* parent_) 
    :x(x_), y(y_), g(g_), h(h_), c(c_), b(b_), direction(direction_), lossP(lossP_), parent(parent_) {
        alpha = 15;
        beta = 8;
        gamma = 10;
    };

int AStarAlgorithm::Node::f() const{
    int propagation_loss = alpha * (*lossP)["propagation"] * (g + h);
    int crossing_loss = beta * (*lossP)["crossing"] * c;
    int bending_loss = gamma * (*lossP)["bending"] * b;
    return propagation_loss + crossing_loss + bending_loss;
}

bool AStarAlgorithm::Node::operator > (const Node& other) const{
    return f() > other.f();
}

int AStarAlgorithm::heuristic(int x1, int y1, int x2, int y2){
    return abs(x1 - x2) + abs(y1 - y2);
}

AStarAlgorithm::AStarAlgorithm(p grid, um* lossP_)
    :row(grid.first), col(grid.second), lossP(lossP_) {}

vector<p> AStarAlgorithm::execute(p start, p target){
    auto cmp = [](Node* a, Node* b) {
        return *a > *b;
    };
    priority_queue< Node*, vector<Node*>, decltype(cmp) > openList(cmp);
    unordered_map<int, unordered_map<int, bool> > closeList;
    int c = 0;
    if(block[start.first][start.second] != 0) c = block[start.first][start.second] + 1;
    Node* startNode = new Node(start.first, start.second, 0, 
        heuristic(start.first, start.second, target.first, target.second), c, 0, "no", lossP, nullptr);

    openList.push(startNode);
    int dx[] = {1, 0, -1, 0}; // east, south, west, north
    int dy[] = {0, -1, 0, 1}; // east, south, west, north
    while(!openList.empty()){
        Node* current = openList.top();
        openList.pop();
        if(current->x == target.first && current->y == target.second){
            vector<p> path;
            while(current->parent != nullptr){
                path.emplace_back(current->x, current->y);
                current = current->parent;
            }
            path.emplace_back(current->x, current->y);
            reverse(path.begin(), path.end());
            return path;
        }

        closeList[current->x][current->y] = true;
        for(int i=0; i<4; i++){
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];
            if(closeList[nx][ny]) continue;
            if(nx < 0 || ny < 0 || nx >= row || ny >= col) continue;
            string direction;
            switch(i){
                case 0:
                    direction = "east";
                    break;
                case 1:
                    direction = "south";
                    break;
                case 2:
                    direction = "west";
                    break;
                case 3:
                    direction = "north";
                    break;
                default:
                    direction = "no";
            }
            int g = current->g + 1;
            int h = heuristic(nx, ny, target.first, target.second);
            int c = current->c + ((block[nx][ny] == 0) ? 0 : block[nx][ny] + 1);
            int b = current->b;
            string parent_direction = current->direction;
            if(parent_direction == "no") b += 0;
            else if(parent_direction != direction) ++b;
            // cout << "parent: " << parent_direction << ", mine: " << direction << ", parent_b: " << current->b << ", my_b:" << b << endl;

            Node* neighbor = new Node(nx, ny, g, h, c, b, direction, lossP, current);
            openList.push(neighbor);
        }
    }
    return {};
}

void AStarAlgorithm::addBlock(vector<p> pathBlock){
    for(const auto& ep : pathBlock){
        block[ep.first][ep.second] += 1;
    }
}
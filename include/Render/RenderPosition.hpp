#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

/*
    RenderPosition class
    Store object positions, calculate transitions/animations between them based on delta time.
*/

class RenderPosition {
private:
    // maps for storing positions, elapsed time, duration; all in reference to ID.
    std::unordered_map<int, std::pair<double, double>> objectPos;
    std::unordered_map<int, int> parentMap;
    std::unordered_map<int, std::pair<double, double>> animStartPos;
    std::unordered_map<int, std::pair<double, double>> animWishPos;
    std::unordered_map<int, double> elapsed;
    std::unordered_map<int, double> duration;
    // vector of finished animations for clean-up
    std::vector<int> finishedAnims;
    void calcNewPos(int ID);
    // erase animation relevant ID's
    void cleanID(int ID);
public:
    // register object with 
    void registerObjectPos(int ID, double x, double y, int parent = -1);
    void setPos(int ID, double x, double y);
    void setWishPos(int ID, double x, double y, double dur);
    int getParent(int ID);
    void setParent(int ID, int parentID);
    // update all elapsed times, clean up animations if finished
    void update(double dt);
    bool isAtPosition(int ID);
    std::pair<double, double> getPos(int ID);
};
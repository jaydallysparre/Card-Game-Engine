#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

/*
    RenderPosition
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

    double PI = 3.1415926535897932384;
    // vector of finished animations for clean-up
    std::vector<int> finishedAnims;
    void calcNewPos(int ID);

    // erase animation relevant ID's
    void cleanID(int ID);
public:
    void registerObjectPos(int ID, double x, double y, int parent = -1);

    // setters
    void setPos(int ID, double x, double y);
    void setWishPos(int ID, double x, double y, double dur);
    void setParent(int ID, int parentID);

    int getParent(int ID);

    // update all elapsed times, clean up animations if finished
    void update(double dt);

    // check if animation is concluded
    bool isAtPosition(int ID);

    // get position of object based on ID
    std::pair<double, double> getPos(int ID);
};
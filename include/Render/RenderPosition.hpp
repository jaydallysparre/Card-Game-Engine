#pragma once

#include <unordered_map>
#include <utility>
#include <cmath>
#include <vector>

/*
    RenderPosition class
    Store object positions, calculate transitions/animations between them based on delta time.
*/

class RenderPosition {
private:
    // maps for storing positions, elapsed time, duration; all in reference to ID.
    std::unordered_map<int, std::pair<double, double>> objectPos;
    std::unordered_map<int, std::pair<double, double>> animStartPos;
    std::unordered_map<int, std::pair<double, double>> animWishPos;
    std::unordered_map<int, double> elapsed;
    std::unordered_map<int, double> duration;

    // vector of finished animations for clean-up
    std::vector<int> finishedAnims;

    void calcNewPos(int ID) {
        // Cancel animation when time elapsed is sufficient.
        if (elapsed[ID] >= duration[ID]) {
            objectPos[ID] = animWishPos[ID];
            finishedAnims.push_back(ID);
            return;
        }
        
        // Easing factor, using sin for smooth transitions.
        double ease = std::sin(M_PI * elapsed[ID] / (2 * duration[ID]));

        objectPos[ID].first = animStartPos[ID].first + (animWishPos[ID].first - animStartPos[ID].first) * ease;
        objectPos[ID].second = animStartPos[ID].second + (animWishPos[ID].second - animStartPos[ID].second) * ease;
    }

    void cleanID(int ID) {
        animStartPos.erase(ID);
        animWishPos.erase(ID);
        elapsed.erase(ID);
        duration.erase(ID);
    }

public:
    void registerObjectPos(int ID, double x, double y) {
        objectPos[ID] = {x, y};
    }
    
    void setWishPos(int ID, double x, double y, double dur) {
        animStartPos[ID] = objectPos[ID];
        animWishPos[ID] = {x, y};
        duration[ID] = dur;
    }

    void setPos(int ID, double x, double y) {
        objectPos[ID] = {x, y};
        cleanID(ID);
    }
    
    void update(double dt) {
        for (auto [ID, wishPos]: animWishPos) {
            elapsed[ID] += dt;
            calcNewPos(ID);
        }

        // clean-up finished animations
        for (int ID: finishedAnims) {
            cleanID(ID);
        }
    }
    
    bool isAtPosition(int ID) {
        return !animWishPos.count(ID);
    }
    
    std::pair<double, double> getPos(int ID) {
        return objectPos[ID];
    }
};
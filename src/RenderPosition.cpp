#include "RenderPosition.hpp"
#include <cmath>

void RenderPosition::calcNewPos(int ID) {
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

void RenderPosition::cleanID(int ID) {
    animStartPos.erase(ID);
    animWishPos.erase(ID);
    elapsed.erase(ID);
    duration.erase(ID);
}

void RenderPosition::registerObjectPos(int ID, double x, double y, int parent) {
    objectPos[ID] = {x, y};
    parentMap[ID] = parent;
}

void RenderPosition::setPos(int ID, double x, double y) {
    objectPos[ID] = {x, y};
    cleanID(ID);
}

void RenderPosition::setWishPos(int ID, double x, double y, double dur) {
    animStartPos[ID] = objectPos[ID];
    animWishPos[ID] = {x, y};
    duration[ID] = dur;
}

int RenderPosition::getParent(int ID) {
    return parentMap[ID];
}

void RenderPosition::setParent(int ID, int parentID) {
    parentMap[ID] = parentID;
}

void RenderPosition::update(double dt) {
    for (auto [ID, wishPos]: animWishPos) {
        elapsed[ID] += dt;
        calcNewPos(ID);
    }

    // clean-up finished animations
    for (int ID: finishedAnims) {
        cleanID(ID);
    }
    finishedAnims.clear();
}

bool RenderPosition::isAtPosition(int ID) {
    return !animWishPos.count(ID);
}

std::pair<double, double> RenderPosition::getPos(int ID) {
    return objectPos[ID];
}
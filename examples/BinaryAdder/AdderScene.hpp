#pragma once

#include "Scene.hpp"
#include "State.hpp"
#include <vector>

class InputState;


class AdderScene : public Scene {
    friend class InputState;
public:
    ObjectId pullDeck;
    ObjectId addBtn;
    std::vector<ObjectId> num1;
    std::vector<ObjectId> num2;
    std::vector<ObjectId> res;

    AdderScene(EventManager& em);
};
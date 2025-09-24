#pragma once

#include <memory>
#include <functional>
#include <string>
#include <unordered_map>
#include <stdexcept>

#include "State.hpp"

/*
    Scene
    Abstract class that represents games and menus. Scenes are in essence a state machine for 
    game/menu logic.
*/

// Alias for State factories; aim to use registration system for State instantiation.
using StateFactory = std::function<std::unique_ptr<State>()>;

class Scene {
    std::unordered_map<std::string, StateFactory> factories;
    std::unique_ptr<State> currentState;
public:
    void run();

    void changeState(std::string newState) {
        if (!factories.count(newState)) {
            throw std::invalid_argument("State " + newState + " not registered. Did you make a typo?");
        }

        currentState = factories[newState]();
    }
};
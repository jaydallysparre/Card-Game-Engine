#pragma once

#include <memory>
#include <functional>
#include <string>
#include <unordered_map>
#include <stdexcept>

#include "State.hpp"
#include "EventManager.hpp"

/*
    Scene
    Abstract class that represents games and menus. Scenes are in essence a state machine for 
    game/menu logic.
*/

// alias for State factories; aim to use registration system for State instantiation.
using StateFactory = std::function<std::unique_ptr<State>()>;

class Scene {
private:
    // the currentState to execute
    std::unique_ptr<State> currentState;

    // reference to the EventManager for communication
    EventManager& eventManager;

protected:
    /// state factory registry 
    std::unordered_map<std::string, StateFactory> factories;

public:
    Scene(EventManager& em) : eventManager(em) {}

    void run() {
        if (!currentState)
            throw std::runtime_error("No state at runtime\n");

        currentState->run();

        // TODO: find way to get events from currentState, and dispatch to EventManager.
    }

    void changeState(const std::string& newState) {
        if (!factories.count(newState)) {
            throw std::invalid_argument("State " + newState + " not registered. Did you make a typo?");
        }

        currentState = factories[newState]();
    }
};
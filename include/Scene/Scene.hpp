#pragma once

#include <memory>
#include <functional>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <climits>
#include <limits>

#include "State.hpp"
#include "EventManager.hpp"
#include "SceneObject.hpp"
#include "ObjectPoolViews.hpp"
#include "Factory.hpp"


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
    // scene view
    ObjectPoolSceneView sceneView;
    // controller view
    ObjectPoolControllerView ctrlView;
    // pool
    ObjectPool pool;
    // factory for building objects
    Factory objFactory;
    // T factor (intended for time, steps, etc.)
    double tFactor = 0.0;
public:
    Scene(EventManager& em) : eventManager(em), sceneView{&pool}, ctrlView{&pool}, objFactory(sceneView) {}
    ObjectPoolSceneView& getSceneView() { return sceneView; }
    const ObjectPoolControllerView& getControllerView() const { return ctrlView; }
    // Main game loop function, called every time
    void run();
    // Change state
    void changeState(const std::string& newState);
    // Move card from deck to deck
    void moveCard(int ID, int fromID, int toID);
    double getTFactor();
    void setTFactor(double newtFactor);
    void updateTFactor(double tFactorAdd);
    //Receive from Auth event from controller, then execute the relevant Req events
    void receiveAndRespond();
};
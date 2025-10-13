#pragma once

#include <memory>
#include <functional>
#include <string>
#include <unordered_map>
#include <stdexcept>

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

public:
    Scene(EventManager& em) : eventManager(em), sceneView{&pool}, ctrlView{&pool}, objFactory(sceneView) {}

    ObjectPoolSceneView& getSceneView() { return sceneView; }
    const ObjectPoolControllerView& getControllerView() const { return ctrlView; }

    // Main game loop function, called every time
    void run() {
        // Process all incoming events and delegate them to the current state.
        receiveAndRespond();

        // Execute the current state's main logic for this frame.
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

    void receiveAndRespond() {
        // get current events, call checks on state
        // if we switch states, run new state
        // forward events to event manager
        
        if (!currentState) return;
        
        std::vector<std::unique_ptr<RequestEvent>> newRequestEvents;

        // Get current authoritative events, call checks on state.
        // Loop through all pending authoritative events from the manager.
        while (eventManager.hasAuthEvents()) {
            std::unique_ptr<AuthoritativeEvent> authEvent = eventManager.popAuthEvent();
            if (!authEvent) continue;

             // Let the current state handle the event. This might trigger a state change
             // and will return any new RequestEvents that were created.
             auto generatedEvents = currentState->handleEvent(*authEvent);

             // Collect all newly generated request events.
             for (auto& req : generatedEvents) {
                 newRequestEvents.push_back(std::move(req));
             }
         }

         // Forward new request events to the event manager.
         for (auto& reqEvent : newRequestEvents) {
             eventManager.pushReqEvent(std::move(reqEvent));
         }
    }
};
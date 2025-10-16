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

    void moveCard(int ID, int fromID, int toID) {
        Deck* fromDeck = static_cast<Deck*>(sceneView.getPointer(fromID));
        fromDeck->removeCard(ID);
        Deck* toDeck = static_cast<Deck*>(sceneView.getPointer(toID));
        toDeck->addCard(ID);

        // update parent
        sceneView.setParent(ID, toID);

        std::cout << "Dragged card " << ID << " from deck " << fromID << " to deck " << toID << "\n";

        auto movedCard = std::make_unique<MovedCard>(ID, fromID, toID);
        eventManager.pushAuthEvent(std::move(movedCard));
    }
    
    double getTFactor() {
        return tFactor;
    }
    
    void setTFactor(double newtFactor) {
        tFactor = newtFactor;
    }

    void updateTFactor(double tFactorAdd) {
        tFactor += tFactorAdd;
    }

    void receiveAndRespond() {
        while (eventManager.hasReqEvents()) {
            // receive event from event manager
            auto event = eventManager.popReqEvent();

            // dispatch correct event handler function
            switch (event->eventType) {
                case ReqEvent::MoveCard: {
                    MoveCard* ev = static_cast<MoveCard*>(event.get());
                    moveCard(ev->ID, ev->fromID, ev->toID);
                    currentState->handleEvent(std::move(event));
                    break;
                }
                case ReqEvent::UpdateTFactor: {
                    UpdateTFactor* ev = static_cast<UpdateTFactor*>(event.get());
                    updateTFactor(ev->newFactor);
                    break;
                }
            }
        }
    }
};
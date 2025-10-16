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
#include <iostream>
#include <climits>

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
    }

    // Change state
    void changeState(const std::string& newState) {
        if (!factories.count(newState)) {
            throw std::invalid_argument("State " + newState + " not registered. Did you make a typo?");
        }

        currentState = factories[newState]();
        currentState->run();
    }

    // Move card from deck to deck
    void moveCard(int ID, int fromID, int toID) {
        PoolObject* fromObj = sceneView.getPointer(fromID);
        PoolObject* toObj = sceneView.getPointer(toID);

        // both hand and deck inherits cardpool, so cast to cardpool type
        CardPool* fromPool = dynamic_cast<CardPool*>(fromObj);
        CardPool* toPool = dynamic_cast<CardPool*>(toObj);

        if (!fromPool || !toPool) {
            auto msg = std::make_unique<StatusMsg>("Invalid card container");
            eventManager.pushAuthEvent(std::move(msg));
            return;
        }

        if (!toObj->tags & TAG_RECEIVABLE) {
            auto msg = std::make_unique<StatusMsg>("This container cannot receive cards");
            eventManager.pushAuthEvent(std::move(msg));
            return;
        }

        // handle if hand is full
        if (toObj->type() == ObjType::Hand) {
            Hand* hand = static_cast<Hand*>(toObj);
            if (hand->getCards().size() >= 10) {
                auto msg = std::make_unique<StatusMsg>("Hand is full (Max 10 cards)");
                eventManager.pushAuthEvent(std::move(msg));
                return;
            }
        }

        if (fromPool->removeCard(ID)) {
            toPool->addCard(ID);
            sceneView.setParent(ID, toID);
            auto movedCard = std::make_unique<MovedCard>(ID, fromID, toID);
            eventManager.pushAuthEvent(std::move(movedCard));
        }
    }
    
    //Receive from Auth event from controller, then execute the relevant Req events
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
            }
        }
    }
};
#include "Scene.hpp"

void Scene::run() {
    // Execute the current state's main logic for this frame.
    if (!currentState)
        throw std::runtime_error("No state at runtime\n");

    currentState->run();
    // Process all incoming events and delegate them to the current state.
    receiveAndRespond();
}

void Scene::changeState(const std::string& newState) {
    if (!factories.count(newState)) {
        throw std::invalid_argument("State " + newState + " not registered. Did you make a typo?");
    }
    currentState = factories[newState]();
}

void Scene::moveCard(int ID, int fromID, int toID) {
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

double Scene::getTFactor() {
    return tFactor;
}

void Scene::setTFactor(double newtFactor) {
    tFactor = newtFactor;
}

void Scene::updateTFactor(double tFactorAdd) {
    tFactor += tFactorAdd;
}

void Scene::receiveAndRespond() {
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
            case ReqEvent::PressButton: {
                PressButton* ev = static_cast<PressButton*>(event.get());
                // press button
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
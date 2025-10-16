#pragma once

#include <string>
#include "EventManager.hpp"
#include "ObjectPoolViews.hpp"

/*
    Controller class
    Abstract class for a class that listens to events sent by the eventManager, and sends responses and interactions.
*/

class Controller {
protected:
    EventManager& em;
    ObjectPoolControllerView poolView_{};
    // event response functions
    // implemented empty to make overriding optional; controller may decide not to implement.
    virtual void movedCard(int cardID, int fromID, int toID) {}
    virtual void createdObject(int ID, double x, double y) {}
    virtual void disabledObject(int ID) {}
    virtual void movedObject(int ID, double x, double y) {}
    virtual void shuffled(int deckID) {}
    virtual void toggledObject(int ID) {}
    virtual void flippedCard(int ID) {}
    virtual void setDescriptor(int ID, std::string str) {}
    virtual void statusMsg(std::string msg) {}

    // lets us stop scene from running if busy
    virtual bool isBusy() = 0;
public:
    Controller(EventManager& em) : em(em) {}

    // Application call after scene switch
    void attachPool(const ObjectPoolControllerView& view) { poolView_ = view; }
    // receive and respond to events
    void receiveAndRespond();

    // init, or re-init controller
    virtual void init(ObjectPoolControllerView objPool);
};
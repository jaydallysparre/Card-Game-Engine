#pragma once

#include "EventManager.hpp"
#include "ObjectPoolViews.hpp"

/*
    Controller class
    Abstract class for a class that listens to events sent by the eventManager, and sends responses and interactions.
*/

class Controller {
private:
    EventManager& em;
protected:
    ObjectPoolControllerView objPool;

    // event response functions
    // implemented empty to make overriding optional; controller may decide not to implement.
    virtual void movedCard(int cardID, int fromID, int toID) {}
    virtual void createdObject(int ID, double x, double y) {}
    virtual void disabledObject(int ID) {}
    virtual void movedObject(int ID, double x, double y) {}
    virtual void shuffled(int deckID) {}
    virtual void toggledObject(int ID) {}
    virtual void flippedCard(int ID) {}

    // lets us stop scene from running if busy
    virtual bool isBusy() = 0;
public:
    Controller(EventManager& em) : em(em) {}

    // receive and respond to events
    void receiveAndRespond();

    // init, or re-init controller
    virtual void init(ObjectPoolControllerView objPool);
};
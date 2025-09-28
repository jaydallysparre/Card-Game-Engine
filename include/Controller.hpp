#pragma once

#include "EventManager.hpp"

/*
    Controller class
    Abstract class for a class that listens to events sent by the eventManager, and sends responses and interactions.
*/

class Controller {
    EventManager& em;
protected:
    // event response functions
    // implemented empty to make overriding optional; controller may decide not to implement.
    virtual void movedCard(int cardID, int fromID, int toID) {}
    virtual void createdObject(int ID) {}
    virtual void disabledObject(int ID) {}
    virtual void shuffled(int deckID) {}
    virtual void toggledObject(int ID) {}
    virtual void flippedCard(int ID) {}
public:
    Controller(EventManager& em) : em(em) {}

    // receive and respond to events
    void receiveAndRespond();
};
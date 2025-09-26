#pragma once

#include "EventManager.hpp"

/*
    Controller class
    Abstract class for a class that listens to events sent by the eventManager, and sends responses and interactions.
*/

class Controller {
    EventManager& eventManager;
public:
    Controller(EventManager& em) : eventManager(em) {}
    void update() {}
};
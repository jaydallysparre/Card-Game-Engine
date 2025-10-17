#pragma once

#include <queue>
#include <memory>
#include "Event.hpp"

/*
    EventManager
    Class that mediates events sent from scene to the controller, and vice-versa.
*/

class EventManager {
private:
    std::queue<std::unique_ptr<AuthoritativeEvent>> authEvents;
    std::queue<std::unique_ptr<RequestEvent>> reqEvents;
public:
    void pushAuthEvent(std::unique_ptr<AuthoritativeEvent> e);
    void pushReqEvent(std::unique_ptr<RequestEvent> e);
    std::unique_ptr<AuthoritativeEvent> popAuthEvent();
    std::unique_ptr<RequestEvent> popReqEvent();
    bool hasAuthEvents();
    bool hasReqEvents();
};
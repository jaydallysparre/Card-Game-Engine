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
    void pushAuthEvent(std::unique_ptr<AuthoritativeEvent> e) {
        authEvents.push(std::move(e));
    }

    void pushReqEvent(std::unique_ptr<RequestEvent> e) {
        reqEvents.push(std::move(e));
    }

    std::unique_ptr<AuthoritativeEvent> popAuthEvent() {
        auto e = std::move(authEvents.front());
        authEvents.pop();
        return e;
    }

    std::unique_ptr<RequestEvent> popReqEvent() {
        auto e = std::move(reqEvents.front());
        reqEvents.pop();
        return e;
    }

    bool hasAuthEvents() {
        return !authEvents.empty();
    }

    bool hasReqEvents() {
        return !reqEvents.empty();
    }
};

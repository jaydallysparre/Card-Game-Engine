#include "EventManager.hpp"

void EventManager::pushAuthEvent(std::unique_ptr<AuthoritativeEvent> e) {
    authEvents.push(std::move(e));
}

void EventManager::pushReqEvent(std::unique_ptr<RequestEvent> e) {
    reqEvents.push(std::move(e));
}

std::unique_ptr<AuthoritativeEvent> EventManager::popAuthEvent() {
    auto e = std::move(authEvents.front());
    authEvents.pop();
    return e;
}

std::unique_ptr<RequestEvent> EventManager::popReqEvent() {
    auto e = std::move(reqEvents.front());
    reqEvents.pop();
    return e;
}

bool EventManager::hasAuthEvents() {
    return !authEvents.empty();
}

bool EventManager::hasReqEvents() {
    return !reqEvents.empty();
}
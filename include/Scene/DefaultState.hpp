#pragma once
#include "State.hpp"
#include <string>
#include <iostream>

class DefaultState : public State {
public:
    DefaultState(Scene& scene_ref, EventManager& em_ref, ObjectPoolSceneView& sv);
    virtual void run();
    virtual void handleEvent(std::unique_ptr<RequestEvent> ev);
};
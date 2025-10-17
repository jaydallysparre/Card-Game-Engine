#pragma once
#include "State.hpp"
#include "Scene.hpp"
#include <memory>

class AdderScene; 

class InputState : public State {
public:
    InputState(Scene& scene_ref, EventManager& em_ref, ObjectPoolSceneView& sv)
        : State(scene_ref, em_ref, sv) {}

    void handleEvent(std::unique_ptr<RequestEvent> ev) override;
};
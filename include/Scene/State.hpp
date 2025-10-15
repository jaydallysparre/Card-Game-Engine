#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "Scene.hpp"
#include "Event.hpp"
#include "EventManager.hpp"

struct Scene;
class State { 
protected:
    Scene& scene;
    EventManager& em;
public:
    State(Scene& scene_ref, EventManager& em_ref) : scene(scene_ref), em(em_ref) {}
    virtual ~State() = default;
    // run our state code upon changing states
    virtual void run() = 0;

    // handle any incoming events; ran continuously in response to new events
    virtual void handleEvent(RequestEvent ev) = 0;
};
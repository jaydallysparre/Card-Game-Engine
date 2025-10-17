#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "Event.hpp"
#include "EventManager.hpp"
#include "ObjectPoolViews.hpp"

/*
    State Class
    We aim to represent games as state machines, hence the state class represents
    some individual game state; we can run code on construction, update periodically,
    and respond to certain events.
*/

class Scene;
class State { 
protected:
    // references to objects we want to communicate with
    Scene& scene;
    EventManager& em;
    ObjectPoolSceneView& sceneView;
public:
    State(Scene& scene_ref, EventManager& em_ref, ObjectPoolSceneView& sv) : 
        scene(scene_ref), em(em_ref), sceneView(sv) {}

    virtual ~State() = default;

    // update any state stuff
    virtual void run() {};

    // handle any incoming events; ran continuously in response to new events
    virtual void handleEvent(std::unique_ptr<RequestEvent> ev) {};
};
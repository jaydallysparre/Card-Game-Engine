#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "Scene.hpp"
#include "Event.hpp"

struct Scene;
struct Event;
struct Substate {
    virtual ~Substate() = default;
    virtual const char* name() const = 0;
    virtual void onEnter(Scene& scene) {}
    virtual void onExit(Scene& scene) {}
    // handles a specific event type and returns a vector of new request events
    virtual std::vector<std::unique_ptr<RequestEvent>> handle(Scene& scene, const AuthoritativeEvent& event) = 0;
};

#include <vector>

class State { 
protected:
    Scene& scene;
    std::vector<std::unique_ptr<Substate>> substates; 
    Substate* currentSubstate = nullptr;
    void addSubstate(std::unique_ptr<Substate> sub) {
        substates.push_back(std::move(sub));
    }  
public:
    State(Scene& scene_ref) : scene(scene_ref) {}
    virtual ~State() = default;
    virtual void run() = 0;
    // Takes an AuthoritativeEvent and returns a vector of unique_ptrs to RequestEvents
    virtual std::vector<std::unique_ptr<RequestEvent>> handleEvent(const AuthoritativeEvent& event) {
        if (currentSubstate) {
            return currentSubstate->handle(scene, event);
        }
        return {}; // Return an empty vector if no substate is active
    }
    void changeSubstate(const std::string& name) {
        // Find the new substate in our list.
        Substate* nextState = nullptr;
        for (const auto& sub : substates) {
            if (sub->name() == name) {
                nextState = sub.get();
                break;
            }
        }

        if (!nextState) {
            throw std::runtime_error("Substate '" + name + "' not found!");
        }

        // Execute the exit/enter lifecycle methods.
        if (currentSubstate) {
            currentSubstate->onExit(scene);
        }
        currentSubstate = nextState;
        currentSubstate->onEnter(scene);
    }

};
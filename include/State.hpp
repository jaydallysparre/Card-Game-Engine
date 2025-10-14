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
    virtual void onEnter(Scene&) {}
    virtual void onExit(Scene&) {}
    // Return true if handled; false lets Scene try someone else (or reject)
    virtual bool handle(Scene&, const Event&) = 0;
};

#include <vector>

class State {
private:
    
public:
    virtual void run() = 0;
private:
    std::vector<std::unique_ptr<Substate>> substates;
};
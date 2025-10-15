#include "State.hpp"

class DefaultState : public State {
public:
    DefaultState(Scene& scene_ref, EventManager& em_ref) : State(scene_ref, em_ref) {}
    virtual void run() {}
    virtual void handleEvent(std::unique_ptr<RequestEvent> ev) {}
};
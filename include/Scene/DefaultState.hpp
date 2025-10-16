#include "State.hpp"
#include <string>

class DefaultState : public State {
public:
    DefaultState(Scene& scene_ref, EventManager& em_ref) : State(scene_ref, em_ref) {}
    virtual void run() {}
    virtual void handleEvent(std::unique_ptr<RequestEvent> ev) {
        if (ev->eventType == ReqEvent::MoveCard) {
            MoveCard* event = static_cast<MoveCard*>(ev.get());
            std::string msg = "Card " + std::to_string(event->ID) + " moved from " + std::to_string(event->fromID) + " to " +  std::to_string(event->toID);
            auto statusMsg = std::make_unique<StatusMsg>(msg);
            em.pushAuthEvent(std::move(statusMsg));
        }
    }
};
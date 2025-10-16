#include "Controller.hpp"

void Controller::init(ObjectPoolControllerView NewObjPool) {
    poolView_ = NewObjPool;
}


// receive events, dispatch relevant response
// ugly boiler plate but what can you do
void Controller::receiveAndRespond() {
    while (em.hasAuthEvents()) {
            // receive event from event manager
            auto event = em.popAuthEvent();

            // dispatch correct event handler function
            switch (event->eventType) {
                case AuthEvent::MovedCard: {
                    MovedCard* ev = static_cast<MovedCard*>(event.get());
                    movedCard(ev->cardID, ev->fromID, ev->toID);
                    break;
                }
                case AuthEvent::CreatedObject: {
                    CreatedObject* ev = static_cast<CreatedObject*>(event.get());
                    createdObject(ev->ID, ev->x, ev->y);
                    break;
                }
                case AuthEvent::DisabledObject: {
                    DisabledObject* ev = static_cast<DisabledObject*>(event.get());
                    disabledObject(ev->ID);
                    break;
                }
                case AuthEvent::MovedObject: {
                    MovedObject* ev = static_cast<MovedObject*>(event.get());
                    movedObject(ev->ID, ev->x, ev->y);
                    break;
                }
                case AuthEvent::Shuffled: {
                    Shuffled* ev = static_cast<Shuffled*>(event.get());
                    shuffled(ev->ID);
                    break;
                }
                case AuthEvent::ToggledObject: {
                    ToggledObject* ev = static_cast<ToggledObject*>(event.get());
                    toggledObject(ev->ID);
                    break;
                }
                case AuthEvent::FlippedCard: {
                    FlippedCard* ev = static_cast<FlippedCard*>(event.get());
                    flippedCard(ev->ID);
                    break;
                }
                case AuthEvent::SetDescriptor: {
                    SetDescriptor* ev = static_cast<SetDescriptor*>(event.get());
                    setDescriptor(ev->ID, ev->description);
                    break;
                }
                case AuthEvent::StatusMsg: {
                    StatusMsg* ev = static_cast<StatusMsg*>(event.get());
                    statusMsg(ev->status);
                }
            }
    }
}
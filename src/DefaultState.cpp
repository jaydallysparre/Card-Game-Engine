#include "DefaultState.hpp"
#include "Scene.hpp"

DefaultState::DefaultState(Scene& scene_ref, EventManager& em_ref, ObjectPoolSceneView& sv) : State(scene_ref, em_ref, sv) {}

void DefaultState::run() {
    if (scene.getTFactor() >= 2.0) {
        scene.setTFactor(0.0);
        auto decks = sceneView.ofType(ObjType::Deck);
        if (!decks.empty()) {
            Deck* deck = static_cast<Deck*>(sceneView.getPointer(decks[0]));
            deck->shuffle();
        }
    }
}

void DefaultState::handleEvent(std::unique_ptr<RequestEvent> ev) {
    if (ev->eventType == ReqEvent::MoveCard) {
        MoveCard* event = static_cast<MoveCard*>(ev.get());
        std::string msg = "Card " + std::to_string(event->ID) + " moved from " + std::to_string(event->fromID) + " to " +  std::to_string(event->toID);
        auto statusMsg = std::make_unique<StatusMsg>(msg);
        em.pushAuthEvent(std::move(statusMsg));
    } else if (ev->eventType == ReqEvent::PressButton) {
        PressButton* event = static_cast<PressButton*>(ev.get());
        auto decks = sceneView.ofType(ObjType::Deck);
        if (decks.size() >= 2) {
            Deck* deck = static_cast<Deck*>(sceneView.getPointer(decks[1]));
            deck->shuffle();
        }
    }
}
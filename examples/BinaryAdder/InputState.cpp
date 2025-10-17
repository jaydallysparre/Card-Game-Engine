#include "InputState.hpp"
#include "AdderScene.hpp" 

void InputState::handleEvent(std::unique_ptr<RequestEvent> ev) {
    AdderScene& adderRef = static_cast<AdderScene&>(scene);
    switch (ev->eventType) {
        case ReqEvent::MoveCard: {
            MoveCard* event = static_cast<MoveCard*>(ev.get());

            if (event->fromID == event->toID) return;
            
            sceneView.addTag(event->fromID, TAG_RECEIVABLE);

            if (event->toID != adderRef.pullDeck) {
                sceneView.rmTag(event->toID, TAG_RECEIVABLE);
            }

            break;
        }
        case ReqEvent::PressButton: {
            PressButton* event = static_cast<PressButton*>(ev.get());
            if (event->ID != adderRef.addBtn) return;

            for (ObjectId deckId : adderRef.res) {
                Deck* deck = static_cast<Deck*>(sceneView.getPointer(deckId));
                if (!deck) continue;

                for (ObjectId cardId : deck->getCards())
                    sceneView.remove(cardId);
            }

            int carry = 0;
            for (int i = 3; i >= 0; i--) {
                Deck* deck1 = static_cast<Deck*>(sceneView.getPointer(adderRef.num1[i]));
                Deck* deck2 = static_cast<Deck*>(sceneView.getPointer(adderRef.num2[i]));
                Deck* deckRes = static_cast<Deck*>(sceneView.getPointer(adderRef.res[i]));

                if (!deck1 || !deck2 || !deckRes) continue;

                int bit1 = deck1->isEmpty() ? 0 : 1;
                int bit2 = deck2->isEmpty() ? 0 : 1;

                int sum = bit1 + bit2 + carry;
                int resultBit = sum % 2;
                carry = sum / 2;

                // Create card in result deck if bit is 1
                if (resultBit == 1) {
                    auto card = adderRef.objFactory.createCard("S", "A", adderRef.res[i]);
                    em.pushAuthEvent(std::make_unique<CreatedObject>(*card, 0.15 + i * 0.15, 0.75));
                }
            }
        }
    }
}
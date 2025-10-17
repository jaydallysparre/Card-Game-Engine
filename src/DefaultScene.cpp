#include "DefaultScene.hpp"
#include "DefaultState.hpp"

DefaultScene::DefaultScene(EventManager& eventManager) : Scene(eventManager) {
    factories["DEFAULT"] = [&](){
        return std::unique_ptr<State>(std::make_unique<DefaultState>(*this, eventManager, sceneView));
    };
    int deck1ID = objFactory.createDeck();
    int deck2ID = objFactory.createDeck(true, true);
    int deck3ID = objFactory.createDeck(false, false);
    int hand1ID = objFactory.createHand();
    
    eventManager.pushAuthEvent(std::make_unique<CreatedObject>(hand1ID, 0.5, 0.85)); 
    eventManager.pushAuthEvent(std::make_unique<SetDescriptor>(hand1ID, "Player Hand 1"));
    
    int btnID = objFactory.createButton("Shuffle");

    eventManager.pushAuthEvent(std::make_unique<CreatedObject>(deck1ID, 0.25, 0.5));
    eventManager.pushAuthEvent(std::make_unique<CreatedObject>(deck2ID, 0.5, 0.5));
    eventManager.pushAuthEvent(std::make_unique<CreatedObject>(deck3ID, 0.75, 0.5));
    eventManager.pushAuthEvent(std::make_unique<CreatedObject>(btnID, 0.5, 0.69));

    eventManager.pushAuthEvent(std::make_unique<SetDescriptor>(deck1ID, "Deck 1"));
    eventManager.pushAuthEvent(std::make_unique<SetDescriptor>(deck2ID, "Deck 2"));
    eventManager.pushAuthEvent(std::make_unique<SetDescriptor>(deck3ID, "Deck 3"));     
    changeState("DEFAULT");
}

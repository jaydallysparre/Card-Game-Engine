#pragma once 

#include "HiloScene.hpp"
#include "State.hpp"

class SetupState: public State {
    SetupState(Scene& scene_ref, EventManager& em_ref, ObjectPoolSceneView& sv) : State(scene_ref, em_ref, sv) {}
    void run() {
        std::vector<ObjectId> deckIds = sceneView.ofType(ObjType::Deck);
        const Deck* deck0 = static_cast<const Deck*>(view.getPointer(deckIds[0]));
        const Deck* deck1 = static_cast<const Deck*>(view.getPointer(deckIds[1]));
        auto topCard = deck0.topCard();
        if(topCard) {
            // Move the top card of deck0 to deck1.
            sceneView.moveCard(*topCard, deckIds[0], deckIds[1]); // NEED TO ASK ABOUT HOW TO GET TOPCARD ID
            // Change the state to the next state.
            scene.changeState("PlayerInputState");
        } 
        else{
            // If the deck is empty, go to end state.
            scene.changeState("GameEndState");
        }
    }
};

class PlayerInputState: public State {
    // Assign the button ID to both 
    ObjectId pressedButton;
    PlayerInputState(Scene& scene_ref, EventManager& em_ref, ObjectPoolSceneView& sv) : State(scene_ref, em_ref, sv) {}
    
    void run() {
        std::vector<ObjectId> ButtonIds = sceneView.ofType(ObjType::Button);
        const Button* button0 = static_cast<const Button*>(view.getPointer(ButtonIds[0]));
        const Button* button1 = static_cast<const Button*>(view.getPointer(ButtonIds[1]));
        // wait until the button is pressed.  
    }
    void handleEvent(std::unique_ptr<RequestEvent> ev){
        switch (event -> eventType){
            case ReqEvent::PressButton: {
                PressButton* ev = static_cast<PressButton*>(event.get());
                // Store the pressed button's ID into the scene's buttonPressed attribute. 
                pressedButton = ev->ID;
            }
        }
    }
    sceen.buttonPressed = pressedButton;
    scene.changeState("CalculateScore");
};

class CalculateScore: public State {
    CalculateScore(Scene& scene_ref, EventManager& em_ref, ObjectPoolSceneView& sv) : State(scene_ref, em_ref, sv) {}
    void run() {
    std::vector<ObjectId> deckIds = sceneView.ofType(ObjType::Deck);
    std::vector<ObjectId> ButtonIds = sceneView.ofType(ObjType::Button);
    const Deck* deck0 = static_cast<const Deck*>(view.getPointer(deckIds[0]));
    const Deck* deck1 = static_cast<const Deck*>(view.getPointer(deckIds[1]));
    const Deck* deck2 = static_cast<const Deck*>(view.getPointer(deckIds[2]));
    const Button* buttonHigh = static_cast<const Button*>(view.getPointer(ButtonIds[0]));
    const Button* buttonLow = static_cast<const Button*>(view.getPointer(ButtonIds[1]));
    // Move the next card from deck 0 to deck 2.
    auto topCard0 = deck0.topCard();
    moveCard(*topCard0, deckIds[0], deckIds[2]);
    // get the card scores of the top card of deck 1 and 2.
    auto topCard1 = deck1.topCard();
    auto topCard2 = deck2.topCard();

    // calculate the scores of the individual cards. 
    int card1Score = scene.cardScore(*topCard1);
    int card2Score = scene.cardScore(*topCard2);
    
    // Increment the score of player if cardscore of deck 1 is higher than deck 2. and the high button was pressed. 
    if (card1Score > card2Score || *buttonHigh == buttonPressed)
    {scene.playerScore++;}
    // Increment the score of player if cardscore of deck 1 is lower than deck 2. and the low button was pressed. 
    else if(card1Score < card2Score || *buttonLow == buttonPressed)
    {scene.playerScore++;}
    // Move to next state
    scene.changeState("SetupState");
    }
};

class GameEndState: public State {
    SetupState(Scene& scene_ref, EventManager& em_ref, ObjectPoolSceneView& sv) : State(scene_ref, em_ref, sv) {}
    // State for ending the game. 
};
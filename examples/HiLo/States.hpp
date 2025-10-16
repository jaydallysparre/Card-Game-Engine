#pragma once 

#include "HiLoState.hpp"
#include "State.hpp"

class SetupState: public State {
    ObjectPoolSceneView& sceneView = scene.getSceneView();
    std::vector<ObjectID> deckIds = sceneView.ofType(ObjType::Deck);
    const Deck* deck0 = static_cast<const Deck*>(view.getPointer(deckIds[0]));
    const Deck* deck1 = static_cast<const Deck*>(view.getPointer(deckIds[1]));
    auto topCard = deck0.topCard();
    void run() {
        if(topCard) {
            // Move the top card of deck0 to deck1.
            moveCard(*topCard, deckIds[0], deckIds[1]); // NEED TO ASK ABOUT HOW TO GET TOPCARD ID
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
    ObjectPoolSceneView& sceneView = scene.getSceneView();
    std::vector<ObjectID> ButtonIds = sceneView.ofType(ObjType::Button);
    const Deck* deck = static_cast<const Deck*>(view.getPointer(deckIds[]))
    // How would i be able to check what button was pressed? 
    scene.changeState("CalculateScore");
};

class CalculateScore: public State {
    ObjectPoolSceneView& sceneView = scene.getSceneView();
    std::vector<ObjectID> deckIds = sceneView.ofType(ObjType::Deck);
    std::vector<ObjectID> cardIds = sceneView.ofType(ObjType::Card);
    const Deck* deck0 = static_cast<const Deck*>(view.getPointer(deckIds[0]));
    const Deck* deck2 = static_cast<const Deck*>(view.getPointer(deckIds[2]));
    // Move the next card from deck 0 to deck 2.
    auto topCard = deck0.topCard();
    moveCard(*topCard, deckIds[0], deckIds[2]);
    // get the card scores of the top card of deck 0 and 1.
    Card* topCard0 = deck0.topCard();
    Card* topCard1 = deck1.topCard();

    // calculate the scores of the individual cards. 
    int card0Score = scene.cardScore(*topCard0);
    int card1Score = scene.cardScore(*topCard1);
    
    // Increment the score of player if cardscore of deck 1 is higher than deck 0.
    if (card0Score > card1Score || hiLo == 1)
    {scene.playerScore++;}
    else if(card0Score < card1Score || hiLo == 0)
    {scene.playerScore++;}
    scene.changeState("SetupState");
};

class GameEndState: public State {
};
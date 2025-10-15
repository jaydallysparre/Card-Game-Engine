#pragma once 

#include "HiLoState.hpp"
#include "State.hpp"

class SetupState: public State {
    void run(); // Do I need to restate this function?
    if() {
        // If the deck is empty, go to end state.
        scene.changeState("GameEndState");
    } 
    else{
        // Move the top card of deck2 to deck 0.
        // Change the state to the next state.
        scene.changeState("PlayerInputState");
    }
};

class PlayerInputState: public State {
    // Request an event to check if the user moved the card to deck 1 or deck 3.
    if() { // if the card is moved to deck1
        scene.changeState("CalculateScoreHigh");
    }
    else { // card is moved to deck3
        scene.changeState("CalculateScoreLow");
    }
};

class CalculateScoreHigh: public State {
    // get the card scores of the top card of deck 0 and 1.
    // Increment the score of player if cardscore of deck 1 is higher than deck 0.
    scene.changeState("SetupState");
};

class CalculateScoreLow: public State {
    // get the card scores of the top card of deck 0 and 3.
    // Increment the score of player if cardscore of deck 0 is higher than deck 3.
    scene.changeState("SetupState");
};

class GameEndState: public State {
};
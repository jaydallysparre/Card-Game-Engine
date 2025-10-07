#pragma once

#include "Scene.hpp"
#include "SceneObject.hpp"
#include "EventManager.hpp"

enum class GameState {
  SHOW_CARD,
  SHOW_SCORE,
  HIGH_LOW,
  CALCULATE,
  END_GAME
};

class HiLoScene: public Scene{
  // Set all the stuff needed for this game, which is technically one Deck.
  // These are some other counters required for this specific game.
  int hL = 0;
  Card currentCard;
  int playerScore = 0;

  public:
  // Method to set the currentCard
  void setCurrentCard(Card& card) {
    currentCard = card;
  }
  // Method to read the currentCard
  Card returnCurrentCard() {
    return currentCard;
  }
  void incrementPlayerScore() {
    playerScore++;
  }
};

GameState state_showCard(HiLoScene& sceneName){
  if (sceneName.isEmpty()) {   // I need to know how how we're going to set the deck id or something so I can replace sceneName if needed.
    return GameState::SHOW_SCORE;
  }
  else {
    Card topCard = sceneName.topCard(); // Same here how should I get the deck to get the topcard?
    // render topCard for the player
    sceneName.setCurrentCard(topCard);
    return GameState::HIGH_LOW;
  }
}

GameState state_showScore(HiLoScene& sceneName){
  // Render the game score for the palyer
  return GameState::END_GAME;
}

GameState state_highLow(HiLoScene& sceneName){
  // Ask the player to choose between high or low, and assign 1 if high, 0 if low to hL, defined as a variable in scene to keep track of the player's decision
  return GameState::CALCULATE;
}

GameState state_calculate(HiLoScene& sceneName){

  Card firstCard = sceneName.returnCurrentCard();
  Card secondCard = sceneName.topCard(); // Again, need to know how the deck is going to be set to properly use topcard();
  
  int firstCardScore = cardScore(firstCard);
  int secondCardScore = cardScore(secondCard);

  if(secondCardScore > firstCardScore) { // NEED AN AND CASE WHERE WE CALL THE HIGH OR LOW FROM PLAYER INPUT
    sceneName.incrementPlayerScore();
    return GameState::SHOW_CARD;
  }
  else if(secondCardScore < firstCardScore) { // NEED AN AND CASE WHERE WE CALL THE HIGH OR LOW FROM PLAYER INPUT
    sceneName.incrementPlayerScore();
    return GameState::SHOW_CARD;
  }
   else{
    return GameState::SHOW_CARD;
  }
} 
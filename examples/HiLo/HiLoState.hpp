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
  ObjectId buttonPressed;
  // Do I need to change the object type too? or should I just use card here?
  public:
  // Method to set the currentCard
  void setCurrentCard(Card& card) {
    currentCard = card;
  }
  // Method to read the currentCard
  Card returnCurrentCard() {
    return currentCard;
  }
  // Method to let the state functions change the state
  void setGameState(GameState newState) {
    currentState = newState;
  }
  // Read current state
  GameState readGameState() {
    return currentState;
  }

      // Calculate the card scores, was this removed intentionally?
  int cardScore(Card& card) {
      auto suitIterator = std::find(SUITS.begin(), SUITS.end(), card.getSuit());
      auto rankIterator = std::find(RANKS.begin(), RANKS.end(), card.getRank());
      int suitValue = std::distance(SUITS.begin(), suitIterator);
      int rankValue = std::distance(RANKS.begin(), rankIterator);
      return suitValue*13 + rankValue;
  }
};

void state_showCard(HiLoScene& sceneName){
  ObjectPoolSceneView& sceneView = sceneName.getSceneView();
  std::vector<ObjectId> deckIds = sceneView.ofType(ObjType::Deck);  // IS THIS HOW I DO IT?
  const Deck* deck = static_cast<const Deck*>(view.getPointer(deckIds[0]))
  if (deckIds[0].isEmpty()) {   // This should reference the first deck right?
    sceneName.setGameState(GameState::SHOW_SCORE);
  }
  else {
    // Card* card = static_cast<Card*>(pool.getPointer(ID))
    Card topCard = deckIds[0].topCard(); // How should I get the topcard? I tried using the normal topCard function to the deck.
    // render topCard for the player
    sceneName.setCurrentCard(topCard);
    sceneName.setGameState(GameState::HIGH_LOW);
  }
}

void state_showScore(HiLoScene& sceneName){
  // Render the game score for the palyer
  sceneName.setGameState(GameState::END_GAME);
}

void state_highLow(HiLoScene& sceneName){
  // Ask the player to choose between high or low, and assign 1 if high, 0 if low to hL, defined as a variable in scene to keep track of the player's decision
  sceneName.setGameState(GameState::CALCULATE);
}

void state_calculate(HiLoScene& sceneName){

  ObjectPoolSceneView& sceneView = sceneName.getSceneView();
  std::vector<ObjectID> deckIds = sceneView.ofType(ObjType::Deck);
  std::vector<ObjectID> players = sceneView.ofType(ObjType::Player); // This how I do it, right?

  Card firstCard = sceneName.returnCurrentCard();
  Card secondCard = deckIds[0].topCard(); // This is how I use the Deck's method on the deck right?
  
  int firstCardScore = deckIds[0].cardScore(firstCard);
  int secondCardScore = deckIds[0].cardScore(secondCard);

  if(secondCardScore > firstCardScore) { // NEED AN AND CASE WHERE WE CALL THE HIGH OR LOW FROM PLAYER INPUT
    players[0].score++; // Increment the score directly from the player
    sceneName.setGameState(GameState::SHOW_CARD);
  }
  else if(secondCardScore < firstCardScore) { // NEED AN AND CASE WHERE WE CALL THE HIGH OR LOW FROM PLAYER INPUT
    players[0].score++;
    sceneName.setGameState(GameState::SHOW_CARD);
  }
   else{
    sceneName.setGameState(GameState::SHOW_CARD);
  }
} 
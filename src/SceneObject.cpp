#include "SceneObject.hpp"

const std::vector<std::string> Deck::SUITS = {"H", "D", "C","S"};
const std::vector<std::string> Deck::RANKS = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

// Equality operator for the Card class that will help when removing cards
bool operator==(const Card& inDeck, const Card& findingCard){
    return inDeck.getSuit() == findingCard.getSuit() && inDeck.getRank() == findingCard.getRank();
}
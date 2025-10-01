#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <memory>
#include "ObjectPool.hpp"

// The card class
class Card : public, PoolObject {
    // Two 
    private:
    std::string suit;
    std::string rank;

    public: 
    // Default Constructor
    Card()
        : suit(""), rank("") // Initializer list
    {}

    Card(std::string s, std::string r)
        : suit(s), rank(r)
    {}

    // Method to set the suit
    void setSuit(std::string s) {suit = s;}
    // Method to set the Rank
    void setRank(std::string r) {rank = r;}
    // Method to read Suit 
    std::string getSuit() const {return suit;}
    // Method to read Rank
    std::string getRank() const {return rank;}
    // ObjectPool functions
    bool faceUp{false};
    ObjType type() const override {return ObjType::Card;}
};

// Equality operator for the Card class that will help when removing cards
bool operator==(const Card& inDeck, const Card& findingCard){
    return inDeck.getSuit() == findingCard.getSuit() && inDeck.getRank() == findingCard.getRank();
}

class CardPool {
    protected: 
    std::vector<Card> cardPool;
    public:
    // Default constructor
    CardPool() {}
    // Method for adding a card
    void addCard(const Card& newCard) {
        cardPool.push_back(newCard);
    }
    // Method for removing a card
    void removeCard(const Card& targetCard) {
        // Find the matching card using the bool operator we defined in the Card class
        auto removingCard = std::find(cardPool.begin(), cardPool.end(), targetCard);
        if (removingCard != cardPool.end()) {
            cardPool.erase(removingCard);
        }
    }
    // Clear or Discard all cards
    void clear() {
        cardPool.clear();
    }
};

// A class that helps the scene objects' location on the screen
class PositionalSceneObject, public, PoolObject{
    protected: 
    double horizontalPosition = 0.0;
    double verticalPosition = 0.0;

    public:
    void setPosition(double x, double y) {
        if (x < 0)
        {horizontalPosition = 0;}
        if (x > 1)
        {horizontalPosition = 1;}
        if (y < 0)
        {verticalPosition = 0;}
        if (y > 1)
        {verticalPosition = 1;}
    }
};

// Class representing deck 
class Deck : public CardPool, public PositionalSceneObject, public PoolObject {
    private: 
    // Building vectors for the card types 
    static const std::vector<std::string> SUITS;
    static const std::vector<std::string> RANKS;
    public:
    // Method for Building decks
    void buildDeck() {
        // Reset Deck
        clear();
        // Two for loops to go through every pairing
        for(const std::string& s : SUITS){
            for(const std::string& r : RANKS){
                addCard(Card(s, r));
            }
        }
    }
    // Method for adding joker 
    void addJokers() {
        addCard(Card("Coloured", "Joker"));
        addCard(Card("NotColoured", "Joker"));
    }
    // Return the top card of the vector 
    Card topCard() {
        if(cardPool.size() != 0){
            return cardPool.back();
        }
    }
    // Shuffle the vector
    void shuffle() {
        auto rng = std::default_random_engine {};
        std::shuffle(std::begin(cardPool), std::end(cardPool), rng);
    }
    // Extra functions for ObjectPool
    ObjType type() const override {return ObjType::Deck;}
};

const std::vector<std::string> Deck::SUITS = {"H", "D", "C","S"};
const std::vector<std::string> Deck::RANKS = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

class Hand : public CardPool, public PositionalSceneObject, public PoolObject {
    // Method for ObjectPool
    ObjType type() const override {return ObjType::Hand;}
};

class Text : public PositionalSceneObject, public PoolObject {
    private: 
    std::vector<std::string> screenText; 
    public: 
    void addText(std::string newText) {
        screenText.push_back(newText);
    }
    // Method for ObjectPool
    ObjType type() const override {return ObjType::Text;}
};
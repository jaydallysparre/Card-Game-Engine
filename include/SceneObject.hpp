#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <memory>
#include <optional>
#include "ObjectPool.hpp"

// The card class
class Card : public PoolObject {
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

bool operator==(const Card& inDeck, const Card& findingCard);

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
class PositionalSceneObject {
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
    std::optional<Card> topCard() const {
        if(cardPool.size() != 0){
            return cardPool.back();
        }
        return std::nullopt;
    }

    // Return the top 2 cards of the vector
    std::pair<std::optional<Card>, std::optional<Card>> top2Cards() const {
        std::optional<Card> card1, card2;

        if (!cardPool.empty())
            card1 = cardPool.back();
        
        if (cardPool.size() >= 2)
            card2 = *(cardPool.rbegin() + 1);

        return {card1, card2};
    }

    // Shuffle the vector
    void shuffle() {
        std::random_device rand;
        auto rng = std::default_random_engine {rand()};
        std::shuffle(std::begin(cardPool), std::end(cardPool), rng);
    }
    // Extra functions for ObjectPool
    ObjType type() const override {return ObjType::Deck;}
};

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
#pragma once

#include <string>
#include <optional>
#include <random>
#include <algorithm>

#include "PoolObject.hpp"

class Card : public PoolObject {
private:
    std::string suit;
    std::string rank;

public:
    Card(std::string s, std::string r) : suit(s), rank(r) {}

    std::string getSuit() const {return suit;}
    std::string getRank() const {return rank;}
    ObjectId getId() const {return id;}

    ObjType type() const override {return ObjType::Card;}
};

class CardPool {
protected:
    std::vector<ObjectId> cardPool;

public:
    void addCard(ObjectId id) {
        cardPool.push_back(id);
    }

    bool removeCard(ObjectId id) {
        for (auto it = cardPool.begin(); it != cardPool.end(); it++) { 
            if (*it == id) {
                cardPool.erase(it);
                return true;
            }
        }
        return false;
    }

    std::vector<ObjectId> getCards() const {
        return cardPool;
    }
};

class Deck : public CardPool, public PoolObject {
public:

// Return the top card of the vector 
    std::optional<ObjectId> topCard() const {
        if(cardPool.size() != 0){
            return cardPool.back();
        }
        return std::nullopt;
    }

    // Return the top 2 cards of the vector
    std::pair<std::optional<ObjectId>, std::optional<ObjectId>> top2Cards() const {
        std::optional<ObjectId> card1, card2;

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

class Hand : public CardPool, public PoolObject {
private:
    // store cards id
    std::vector<ObjectId> cards;
public:
    void addCardId(ObjectId cardId) {cards.push_back(cardId);}
    void removeCardId(ObjectId cardId) {cards.erase(std::remove(cards.begin(), cards.end(), cardId), cards.end());}
    const std::vector<ObjectId>& getCards() const {return cards;}
    // Method for ObjectPool
    ObjType type() const override {return ObjType::Hand;}
};

class Text : public PoolObject {
    private: 
    std::vector<std::string> screenText; 
    public: 
    void addText(std::string newText) {
        screenText.push_back(newText);
    }
    // Method for ObjectPool
    ObjType type() const override {return ObjType::Text;}
};

class Player : public PoolObject {
public:
    ObjType type() const override {return ObjType::Player;}
    std::string name;
    ObjectId hand{};
    int score{0};
};

class Button : public PoolObject {

};
#pragma once

#include <string>
#include <optional>
#include <random>
#include <algorithm>

#include "PoolObject.hpp"

/*
    Objects file
    Here we define all relevant objects for the games.
*/


/*
    Card
    Represents cards, has a suit and rank.
*/

class Card : public PoolObject {
private:
    std::string suit;
    std::string rank;
public:
    Card(std::string s, std::string r) : suit(s), rank(r) {}
    std::string getSuit() const {return suit;}
    std::string getRank() const {return rank;}
    ObjType type() const override {return ObjType::Card;}
};

/*
    Card Pool
    Base class for
*/
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
    std::pair<std::optional<ObjectId>, std::optional<ObjectId>> top2Cards() const;
    // Shuffle the vector
    void shuffle();
    // Extra functions for ObjectPool
    ObjType type() const override {return ObjType::Deck;}
};

class Hand : public CardPool, public PoolObject {
public:
    // // Method for ObjectPool
    ObjType type() const override {return ObjType::Hand;}
};

class Text : public PoolObject {
    private: 
    std::vector<std::string> screenText; 
    public: 
    void addText(std::string newText);
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
public:
    std::string text;
    Button(std::string text) : text(text) {}
    ObjType type() const override {return ObjType::Button;}
};
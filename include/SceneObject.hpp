#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

// The card class
class Card {
    // Two 
    private:
    string Suit;
    string Rank;

    public: 
    // Default Constructor
    Card()
        : Suit(""), Rank("") // Initializer list
    {}

    Card(string s, string r)
        : Suit(s), Rank(r)
    {}

    // Method to set the suit
    void SetSuit(string s) {Suit = s;}
    // Method to set the Rank
    void SetRank(string r) {Rank = r;}
    // Method to read Suit 
    string GetSuit() const {return Suit;}
    // Method to read Rank
    string GetRank() const {return Rank;}
    // Method to print card (written just in case)
    void PrintCard() const {cout << Suit << " " << Rank;}
};

// Equality operator for the Card class that will help when removing cards
bool operator==(const Card& inDeck, const Card& findingCard){
    return inDeck.GetSuit() == findingCard.GetSuit() && inDeck.GetRank() == findingCard.GetRank();
}

class CardPool {
    protected: 
    vector<Card> cardpool;
    public:
    // Default constructor
    CardPool() {}
    // Method for adding a card
    void AddCard(const Card& newcard) {
        cardpool.push_back(newcard);
    }
    // Method for removing a card
    void RemoveCard(const Card& removecard) {
        // Find the matching card using the bool operator we defined in the Card class
        auto TargetCard = find(cardpool.begin(), cardpool.end(), removecard);
        if (TargetCard != cardpool.end()) {
            cardpool.erase(TargetCard);
        }
    }
    // Clear or Discard all cards
    void Clear() {
        cardpool.clear();
    }

};

// Class representing deck 
class Deck : public CardPool {
    private: 
    // Building vectors for the card types 
    vector<string> Suits = {"H", "D", "C","S"};
    vector<string> Ranks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    public:
    // Method for Building decks
    void BuildDeck() {
        // Reset Deck
        cardpool.clear();
        // Two for loops to go through every pairing
        for(const string& s : Suits){
            for(const string& r : Ranks){
                cardpool.push_back(Card(s, r));
            }
        }
    }
    // Method for adding joker 
    void AddJokers() {
        cardpool.push_back(Card("Coloured", "Joker"));
        cardpool.push_back(Card("NotColoured", "Joker"));
    }
};

class Hand : public CardPool {
};

class SceneObject {
public:
};

// A class that inherits from scene object that has positional arguments
class PositionalSceneObject: public SceneObject {
public: 

};
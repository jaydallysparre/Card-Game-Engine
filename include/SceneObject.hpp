#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <memory>

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

    // Print the cards within the vector
    void PrintCards() const {
        for(const auto& card : cardpool) {
            card.PrintCard();
            cout << " ";
        }
        cout << endl;
    }
};

// A class that helps the scene objects' location on the screen
class PositionalSceneObject{
    protected: 
    double HorizontalPosition = 0.0;
    double VerticalPosition = 0.0;

    public:
    void SetPosition(double x, double y) {
        HorizontalPosition = x;
        VerticalPosition = y;
    }

    void SetHorizontalPosition(double x) {
        HorizontalPosition = x;
    }

    void SetVerticalPosition(double y) {
        VerticalPosition = y;
    }
};

// Class representing deck 
class Deck : public CardPool, public PositionalSceneObject {
    private: 
    // Building vectors for the card types 
    static const vector<string> Suits;
    static const vector<string> Ranks;
    public:
    // Method for Building decks
    void BuildDeck() {
        // Reset Deck
        Clear();
        // Two for loops to go through every pairing
        for(const string& s : Suits){
            for(const string& r : Ranks){
                AddCard(Card(s, r));
            }
        }
    }
    // Method for adding joker 
    void AddJokers() {
        AddCard(Card("Coloured", "Joker"));
        AddCard(Card("NotColoured", "Joker"));
    }
};

const vector<string> Deck::Suits = {"H", "D", "C","S"};
const vector<string> Deck::Ranks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

class Hand : public CardPool, public PositionalSceneObject {
};

class Text : public PositionalSceneObject {
    private: 
    vector<string> ScreenText; 
    public: 
    void AddText(string newText) {
        ScreenText.push_back(newText);
    }
};

class SceneObject {
    private:
    // Use smart pointers for better memory allocation
    vector<unique_ptr<PositionalSceneObject>> Objects;
    public:
    void AddObject(unique_ptr<PositionalSceneObject> NewObject) {
        Objects.push_back(move(NewObject));
    }
};

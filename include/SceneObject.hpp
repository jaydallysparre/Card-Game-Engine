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

// Class representing deck 
class Deck {
    private: 
    // Building a vector for the deck
    vector<Card> deck = {};
    // Building vectors for the card types 
    vector<string> Suits = {"H", "D", "C","S"};
    vector<string> Ranks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    public:
    // Default constructor
    Deck()
    {}
    // Method for Building decks
    void BuildDeck() {
        // Reset Deck
        deck.clear();

        // Two for loops to go through every pairing
        for(const string& s : Suits){
            for(const string& r : Ranks){
                deck.push_back(Card(s, r));
            }
        }
    }
    // Method for adding joker 
    void AddJokers() {
        deck.push_back(Card("Coloured", "Joker"));
        deck.push_back(Card("NotColoured", "Joker"));
    }
    // Method for adding a card
    void AddCard(string s, string r) {
        deck.push_back(Card(s, r));
    }
    // Method for removing a card
    void RemoveCard(string s, string r) {
        auto TargetCard = find_if(deck.begin(), deck.end(), [&](const Card& card) {
            return card.Suit == s && card.Rank == r;
        });   
        // If the card exists in the deck, erase it. 
        if (TargetCard != deck.end()) {
            deck.erase(it);
        }
    }
};

class Hand : public Deck {

};

class SceneObject {
public:
};

// A class that inherits from scene object that has positional arguments
class PositionalSceneObject: public SceneObject {
public: 

};
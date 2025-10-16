#include "Factory.hpp"

const std::vector<std::string> Factory::SUITS = {"H", "D", "C","S"};
const std::vector<std::string> Factory::RANKS = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

ObjectId Factory::createDeck(bool full, bool shuffled) {
    // create deck object
    auto deckObject = std::make_unique<Deck>();
    ObjectId deckId = view.add(std::move(deckObject));
    view.addTag(deckId, TAG_GRABBABLE | TAG_RECEIVABLE | TAG_RENDERABLE);
    Deck* deck = static_cast<Deck*>(view.getPointer(deckId));

    // generate full deck of cards and add ID's to deck
    if (full) {
        for (const auto& s : SUITS) {
            for (const auto& r: RANKS) {
                auto card = std::make_unique<Card>(s, r);
                ObjectId cardId = view.add(std::move(card));
                deck->addCard(cardId);
                view.setParent(cardId, deckId);
            }
        }
    }

    if (shuffled) {
        deck->shuffle();
    }
    
    return deckId;
}

ObjectId Factory::createButton(std::string text) {
    auto button = std::make_unique<Button>(text);
    ObjectId btnId = view.add(std::move(button));
    return btnId;
}
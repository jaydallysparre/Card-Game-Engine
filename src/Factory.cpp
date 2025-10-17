#include "Factory.hpp"

const std::unordered_set<std::string> Factory::SUITS = {"H", "D", "C","S"};
const std::unordered_set<std::string> Factory::RANKS = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

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

std::optional<ObjectId> Factory::createCard(std::string suit, std::string rank, ObjectId toDeckID) {
    auto* rawDeckPtr = view.getPointer(toDeckID);
    if (!rawDeckPtr) return std::nullopt;

    Deck* deck = static_cast<Deck*>(rawDeckPtr);

    if (!SUITS.count(suit) || !RANKS.count(rank))  return std::nullopt;

    auto cardObject = std::make_unique<Card>(suit, rank);
    ObjectId cardId = view.add(std::move(cardObject));
    
    deck->addCard(cardId);
    view.setParent(cardId, toDeckID);
    view.addTag(cardId, TAG_RENDERABLE);

    return cardId;
}

ObjectId Factory::createHand() {
    auto handObject = std::make_unique<Hand>();
    ObjectId handId = view.add(std::move(handObject));
    view.addTag(handId, TAG_RECEIVABLE | TAG_RENDERABLE | TAG_GRABBABLE);
    return handId;
}

ObjectId Factory::createButton(std::string text) {
    auto button = std::make_unique<Button>(text);
    ObjectId btnId = view.add(std::move(button));
    return btnId;
}

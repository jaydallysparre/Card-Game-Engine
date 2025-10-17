#include "SceneObject.hpp"  

// CardPool Implementation
void CardPool::addCard(ObjectId id) {
    cardPool.push_back(id);
}

bool CardPool::removeCard(ObjectId id) {
    auto it = std::find(cardPool.begin(), cardPool.end(), id);
    if (it != cardPool.end()) {
        cardPool.erase(it);
        return true;
    }
    return false;
}

std::vector<ObjectId> CardPool::getCards() const {
    return cardPool;
}

bool CardPool::isEmpty() {
    return cardPool.empty();
}

// Deck Implementation
std::optional<ObjectId> Deck::topCard() const {
    if (!cardPool.empty()) {
        return cardPool.back();
    }
    return std::nullopt;
}

std::pair<std::optional<ObjectId>, std::optional<ObjectId>> Deck::top2Cards() const {
    std::optional<ObjectId> card1, card2;
    if (!cardPool.empty())
        card1 = cardPool.back();
    if (cardPool.size() >= 2)
        card2 = *(cardPool.rbegin() + 1);
    return {card1, card2};
}

void Deck::shuffle() {
    std::random_device rand;
    auto rng = std::default_random_engine{rand()};
    std::shuffle(std::begin(cardPool), std::end(cardPool), rng);
}

// Text implementation
void Text::addText(std::string newText) {
    screenText.push_back(newText);
}
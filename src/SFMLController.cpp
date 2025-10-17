#include <iostream>
#include "SFMLController.hpp"

SFMLController::SFMLController(EventManager& em) : RenderController(em), renderer(positionHandler, poolView_) {}

bool SFMLController::isBusy() {
    return busy;
}

void SFMLController::handleSFMLEvent(sf::RenderWindow& window, sf::Event& event, float dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    switch (event.type) {
        // Handle drag card (deck/hand to deck/hand)
        case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left) {
                std::optional<ObjectId> cardIDAtPos;
                auto hands = poolView_.ofType(ObjType::Hand);
                for (ObjectId handId : hands) {
                    cardIDAtPos = renderer.getTopCardAtPosInHand(window, mousePos, handId);
                    if (cardIDAtPos) break;
                }
                
                if (!cardIDAtPos) {
                    cardIDAtPos = renderer.getTopCardAtPos(window, mousePos);
                }

                if (cardIDAtPos) {
                    activeDrag = true;
                    busy = true;
                    // Store normalized position
                    currentDragID = *cardIDAtPos;
                }
            
            auto btnIDAtPos = renderer.getButtonAtPos(window, mousePos);

            if (btnIDAtPos) {
                auto pressButton = std::make_unique<PressButton>(*btnIDAtPos);
                em.pushReqEvent(std::move(pressButton));
            }
        }
        break;

        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (activeDrag) {
                    std::optional<ObjectId> targetId;
                    // if we end on a receivable deck, request that we send our dragged card to that deck
                    targetId = renderer.getDeckAtPos(window, mousePos, poolView_.getParent(currentDragID));
                    // if not deck but hand, request that we send our dragged card to hand
                    if (!targetId) {
                        targetId = renderer.getHandAtPos(window, mousePos);
                    }

                    if (targetId && currentDragID != -1) { // and isReceivable
                        auto moveCard = std::make_unique<MoveCard>(currentDragID, poolView_.getParent(currentDragID), *targetId);
                        em.pushReqEvent(std::move(moveCard));
                    }

                    auto parentPos = positionHandler.getPos(poolView_.getParent(currentDragID));
                    // reset card position
                    positionHandler.setWishPos(currentDragID, parentPos.first, parentPos.second, 0.1);
                }

                currentDragID = -1;
                activeDrag = false;
                busy = false;
            }
            break;

        case sf::Event::MouseMoved:
            if (activeDrag) {
                // set position of card in position handler
                positionHandler.setPos(currentDragID,
                    static_cast<double>(mousePos.x)/window.getSize().x,
                    static_cast<double>(mousePos.y)/window.getSize().y);
            }
            break;
        case sf::Event::Resized:
            renderer.calcNewSizes(window);
            break;
    }
}

void SFMLController::render(sf::RenderWindow& window, float dt) {
    positionHandler.update(dt);

    std::optional<ObjectId> draggedCard = (activeDrag ? std::optional(currentDragID) : std::nullopt);
    // grab all decks from the object pool and render them.
    std::vector<ObjectId> decks = poolView_.ofType(ObjType::Deck);
    for (ObjectId deckID : decks) {
        const Deck* deck = dynamic_cast<const Deck*>(poolView_.getPointer(deckID));
        renderer.renderDeck(window, deck, deckID, draggedCard);
    }
    // grab all hands from the object pool and render them.
    std::vector<ObjectId> hands = poolView_.ofType(ObjType::Hand);
    for (int handID : hands) {
        const Hand* hand = dynamic_cast<const Hand*>(poolView_.getPointer(handID));
        renderer.renderHand(window, hand, handID, draggedCard);
    }

    std::vector<ObjectId> buttons = poolView_.ofType(ObjType::Button);
    for (ObjectId btnID : buttons) {
        renderer.renderButton(window, btnID);
    }

    if (activeDrag && currentDragID != -1) {
        renderer.renderCard(window, currentDragID);
    }

    renderer.renderStatus(window, dt);
}

void SFMLController::createdObject(int ID, double x, double y) {
    RenderController::createdObject(ID, x, y);
    const PoolObject* obj = poolView_.getPointer(ID);
    if (obj->type() == ObjType::Button) {
        renderer.createBtn(ID);
    }
}

void SFMLController::movedCard(int cardID, int fromID, int toID) {
    // Check if the destination is a hand
    const auto* toObj = poolView_.getPointer(toID);
    if (toObj && toObj->type() == ObjType::Hand) {
        // A card was moved TO this hand, so update its layout
        recalculateHandLayout(toID);
    }

    // Also check if the card came from a hand
    const auto* fromObj = poolView_.getPointer(fromID);
    if (fromObj && fromObj->type() == ObjType::Hand) {
        // A card was moved FROM this hand, so update its layout too
        recalculateHandLayout(fromID);
    }
    
    // The animation for a card moving to a deck 
    if (toObj && toObj->type() == ObjType::Deck) {
        auto toPos = positionHandler.getPos(toID);
        positionHandler.setWishPos(cardID, toPos.first, toPos.second, 0.5);
    }
}

void SFMLController::setDescriptor(int ID, std::string str) {
    renderer.setDeckLabel(ID, str);
}

void SFMLController::statusMsg(std::string msg) {
    renderer.setStatus(msg);
}

void SFMLController::recalculateHandLayout(int handID) {
    const Hand* hand = dynamic_cast<const Hand*>(poolView_.getPointer(handID));
    if (!hand) return;

    const auto& cards = hand->getCards();
    if (cards.empty()) return;

    // get the base position of the hand itself
    auto handPos = positionHandler.getPos(handID);
    
    // define card spacing (as a fraction of screen width)
    const double CARD_SPACING = 0.05; // 5% of the screen width
    
    // calculate total width of all cards to center them
    double totalWidth = (cards.size() - 1) * CARD_SPACING;
    double startX = handPos.first - (totalWidth / 2.0);

    // loop through cards and assign each a new target position
    int i = 0;
    for (ObjectId cardId : cards) {
        double targetX = startX + (i * CARD_SPACING);
        double targetY = handPos.second;
        
        // use setWishPos to animate the card to its new spot
        positionHandler.setWishPos(cardId, targetX, targetY, 0.3); // 0.3s animation
        i++;
    }
}
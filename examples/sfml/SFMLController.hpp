#pragma once

#include <utility>
#include <SFML/Graphics.hpp>
#include "Controller.hpp"
#include "RenderController.hpp"
#include "SFMLRenderer.hpp"
#include "SceneObject.hpp"
#include <iostream>

class SFMLController : public RenderController {
    SFMLRenderer renderer;
    
    // variables for dragging cards
    bool activeDrag = false;
    int currentDragID = -1;

    // boolean for isBusy()
    bool busy = false;
public:
    SFMLController(EventManager& em) : RenderController(em), renderer(positionHandler, poolView_) {}

    virtual bool isBusy() override {
        return busy;
    }

    void handleSFMLEvent(sf::RenderWindow& window, sf::Event& event, float dt) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        switch (event.type) {
            case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                auto cardIDAtPos = renderer.getTopCardAtPos(window, mousePos);
                if (!cardIDAtPos) {
                    break;
                }
                activeDrag = true;
                busy = true;

                // Store normalized position
                currentDragID = *cardIDAtPos;
            }
            break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (activeDrag) {
                        // if we end on a receivable deck, request that we send our dragged card to that deck
                        auto draggedDeck = renderer.getDeckAtPos(window, mousePos, poolView_.getParent(currentDragID));
                        if (draggedDeck && currentDragID != -1) { // and isReceivable
                            auto moveCard = std::make_unique<MoveCard>(currentDragID, poolView_.getParent(currentDragID), *draggedDeck);
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
        }
    }

    // Render function; get relevant objects, and ID's, forward to the renderer
    void render(sf::RenderWindow& window, float dt) {
        positionHandler.update(dt);

        // grab all decks from the object pool and render them.
        std::vector<ObjectId> decks = poolView_.ofType(ObjType::Deck);
        for (int deckID : decks) {
            const Deck* deck = dynamic_cast<const Deck*>(poolView_.getPointer(deckID));
            renderer.renderDeck(window, deck, deckID);
        }

        renderer.renderStatus(window, dt);
    }

    /*
        Event response functions
    */

    void movedCard(int cardID, int fromID, int toID) override {
        positionHandler.setParent(cardID, toID);
        auto toPos = positionHandler.getPos(toID);
        positionHandler.setWishPos(cardID, toPos.first, toPos.second, 0.5);
    }

    void setDescriptor(int ID, std::string str) override {
        renderer.setDeckLabel(ID, str);
    }

    void statusMsg(std::string msg) override {
        renderer.setStatus(msg);
    }
};
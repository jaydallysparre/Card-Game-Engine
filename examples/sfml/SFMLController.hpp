#pragma once

#include <utility>
#include <SFML/Graphics.hpp>
#include "Controller.hpp"
#include "RenderController.hpp"
#include "SFMLRenderer.hpp"
#include "SceneObject.hpp"

class SFMLController : public RenderController {
    SFMLRenderer renderer;
    
    // variables for dragging cards
    bool activeDrag = false;
    int currentDragID = -1;
    sf::Vector2i originalPos;

    // boolean for isBusy()
    bool busy = false;
public:
    SFMLController(EventManager& em) : RenderController(em), renderer(positionHandler) {
        // temp hacks
        ObjectPool* newPool = new ObjectPool();
        objPool.p = newPool;
        std::unique_ptr<Deck> deck = std::make_unique<Deck>();
        deck->buildDeck();
        int ID = newPool->add(std::move(deck));
        //positionHandler.registerObjectPos(ID, 1.0, 1.0);
    }

    virtual bool isBusy() override {
        return busy;
    }

    void handleSFMLEvent(sf::RenderWindow& window, sf::Event& event, float dt) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        switch (event.type) {
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    activeDrag = true;
                    busy = true;

                    auto cardIDAtPos = renderer.getCardAtPos(window, mousePos);

                    if (!cardIDAtPos) {
                        break;
                    }

                    originalPos = {
                        static_cast<int>(mousePos.x / static_cast<float>(window.getSize().x)),
                        static_cast<int>(mousePos.y / static_cast<float>(window.getSize().y))
                    };

                    currentDragID = *cardIDAtPos;
                }
                break;
            
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (activeDrag) {
                        //int draggedDeck = renderer.getDeckAtPos(window, mousePos);

                        // reset card position
                        positionHandler.setPos(currentDragID, originalPos.x, originalPos.y);
                        originalPos = {};
                        currentDragID = -1;
                    }

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
        std::vector<ObjectId> decks = objPool.ofType(ObjType::Deck);
        for (int deckID : decks) {
            const Deck* deck = dynamic_cast<const Deck*>(objPool.getPointer(deckID));
            renderer.renderDeck(window, deck, deckID);
        }
    }

    /*
        Event response functions
    */

    //void movedCard(int cardId, int fromID, int toID) override;
};
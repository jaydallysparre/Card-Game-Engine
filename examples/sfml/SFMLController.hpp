#pragma once

#include <utility>
#include <SFML/Graphics.hpp>
#include "Controller.hpp"
#include "RenderController.hpp"
#include "SFMLRenderer.hpp"
#include "SceneObject.hpp"

class SFMLController : public RenderController {
    SFMLRenderer renderer;
    bool activeDrag = false;
public:
    SFMLController(EventManager& em) : RenderController(em), renderer(positionHandler) {
        // temp hacks
        ObjectPool* newPool = new ObjectPool();
        objPool.p = newPool;
        std::unique_ptr<Deck> deck = std::make_unique<Deck>();
        deck->buildDeck();
        int ID = newPool->add(std::move(deck));
        positionHandler.registerObjectPos(ID, 0.5, 0.5);
    }

    virtual bool isBusy() override {
        return false;
    }

    void handleSFMLEvent(sf::Event& event, float dt) {
        switch (event.type) {
            case sf::Event::MouseButtonPressed:
                // probably make a distinction between pressing and held here for buttons etc.
                if (event.mouseButton.button == sf::Mouse::Left) {
                    activeDrag = true;
                    // pick object closest to cursor / with highest z index
                }
                break;
            
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (activeDrag) {
                        // If our drag finishes with our mouse on a receivable object, send this as an event
                    }
                    activeDrag = false;
                }
                break;

            case sf::Event::MouseMoved:
                if (activeDrag) {
                    // update object pos to normalized mouse position
                }
                break;
        }
    }

    // temp hack
    float dtSum = 0;

    // Render function; get relevant objects, and ID's, forward to the renderer
    void render(sf::RenderWindow& window, float dt) {
        positionHandler.update(dt);

        // move testing
        dtSum += dt;
        if (dtSum > 2.0) {
            positionHandler.setWishPos(1, 0.2, 0.2, 500.0);
        }

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
#pragma once

#include <SFML/Graphics.hpp>
#include "Controller.hpp"
#include "SFMLRenderer.hpp"

class SFMLController : public Controller {
    SFMLRenderer renderer;
    bool activeDrag = false;
public:
    SFMLController(EventManager& em) : Controller(em) {}

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

    void render(sf::Window& window, float dt) {
        // for Object in however we store objects
        // renderer.renderObject()
    }

    /*
        Event response functions
    */

    //void movedCard(int cardId, int fromID, int toID) override;
};
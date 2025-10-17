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
    // boolean for isBusy()
    bool busy = false;
public:
    SFMLController(EventManager& em);
    bool isBusy() override;
    void handleSFMLEvent(sf::RenderWindow& window, sf::Event& event, float dt);
    // Render function; get relevant objects, and ID's, forward to the renderer
    void render(sf::RenderWindow& window, float dt);
    /*
        Event response functions
    */
    void createdObject(int ID, double x, double y) override;
    void movedCard(int cardID, int fromID, int toID) override;
    void setDescriptor(int ID, std::string str) override;
    void statusMsg(std::string msg) override;
    void recalculateHandLayout(int handID);
};
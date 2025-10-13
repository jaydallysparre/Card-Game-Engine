#pragma once

#include "Scene.hpp"

class DefaultScene : public Scene {
public:
    DefaultScene(EventManager& em) : Scene(em) {
        int deckID = objFactory.createDeck();
        em.pushAuthEvent(std::make_unique<CreatedObject>(deckID, 0.5, 0.5));
    }
};
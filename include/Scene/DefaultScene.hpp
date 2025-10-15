#pragma once

#include "Scene.hpp"
#include "DefaultState.hpp"

class DefaultScene : public Scene {
public:
    DefaultScene(EventManager& em) : Scene(em) {
        factories["DEFAULT"] = [&](){
            return std::unique_ptr<State>(std::make_unique<DefaultState>(*this, em));
        };

        int deck1ID = objFactory.createDeck();
        int deck2ID = objFactory.createDeck(true, true);
        em.pushAuthEvent(std::make_unique<CreatedObject>(deck1ID, 0.25, 0.5));
        em.pushAuthEvent(std::make_unique<CreatedObject>(deck2ID, 0.75, 0.5));

        changeState("DEFAULT");
    }
};
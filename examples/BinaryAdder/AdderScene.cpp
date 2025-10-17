#include "AdderScene.hpp"
#include "InputState.hpp"

AdderScene::AdderScene(EventManager& em)
    : Scene(em), num1(4), num2(4), res(4)
{
    factories["ADDER"] = [&]() {
        return std::make_unique<InputState>(*this, em, sceneView);
    };

    for (int i = 0; i < 4; i++) {
        num1[i] = objFactory.createDeck(false);
        em.pushAuthEvent(std::make_unique<CreatedObject>(num1[i], 0.15+i*0.15, 0.25));

        num2[i] = objFactory.createDeck(false);
        em.pushAuthEvent(std::make_unique<CreatedObject>(num2[i], 0.15+i*0.15, 0.5));

        res[i] = objFactory.createDeck(false);
        em.pushAuthEvent(std::make_unique<CreatedObject>(res[i], 0.15+i*0.15, 0.75));
        sceneView.rmTag(res[i], TAG_RECEIVABLE | TAG_GRABBABLE);
    }

    pullDeck = objFactory.createDeck(false);
    em.pushAuthEvent(std::make_unique<CreatedObject>(pullDeck, 0.8, 0.5));
    em.pushAuthEvent(std::make_unique<SetDescriptor>(pullDeck, "digits"));

    for (int i = 0; i < 8; i++) {
        auto card = objFactory.createCard("S", "A", pullDeck);
        if (card)
            em.pushAuthEvent(std::make_unique<CreatedObject>(*card, 0.8, 0.5));
    }

    addBtn = objFactory.createButton("Add");
    em.pushAuthEvent(std::make_unique<CreatedObject>(addBtn, 0.8, 0.75));

    changeState("ADDER");
}
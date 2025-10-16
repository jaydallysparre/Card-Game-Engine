#pragma once

#include "Controller.hpp"
#include "RenderPosition.hpp"
#include <iostream>

/*
    Render Controller
    Abstract base render controller class with position handler.
*/

class RenderController : public Controller {
protected:
    RenderPosition positionHandler;
public:
    RenderController(EventManager& em) : Controller(em) {}

    void init(ObjectPoolControllerView objPool) override {
        Controller::init(objPool);
        // reset position handler
        positionHandler = RenderPosition();
    }

    void createdObject(int ID, double x, double y) override {
        auto obj = poolView_.getPointer(ID);

        // if object is a deck, then register all of it's cards to the same position
        if (obj->type() == ObjType::Deck) {
            const Deck* deck = static_cast<const Deck*>(obj);

            for (ObjectId cardId: deck->getCards()) {
                positionHandler.registerObjectPos(cardId, x, y, deck->id);
            }
        } else if (obj->type() == ObjType::Hand) {
            const Hand* hand = static_cast<const Hand*>(obj);
            
            for (ObjectId cardId: hand->getCards()) {
                positionHandler.registerObjectPos(cardId, x, y, hand->id);
            }
        }
        positionHandler.registerObjectPos(ID, x, y);
    }
    
    void movedObject(int ID, double x, double y) override {
        positionHandler.setWishPos(ID, x, y, 2.5);
    }
};
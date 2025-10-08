#pragma once

#include "Controller.hpp"
#include "RenderPosition.hpp"

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
        positionHandler.registerObjectPos(ID, x, y);
    }
    
    void movedObject(int ID, double x, double y) override {
        positionHandler.setWishPos(ID, x, y, 2.5);
    }
};
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
    RenderController(EventManager& em);
    void init(ObjectPoolControllerView objPool) override;
    void createdObject(int ID, double x, double y) override;
    void movedObject(int ID, double x, double y) override;
};
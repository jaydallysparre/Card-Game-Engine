#pragma once

#include <memory>
#include "Controller.hpp"
#include "Scene.hpp"
#include "EventManager.hpp"

/*
    Application
    Abstract class that handles creating the window, passing inputs to the controller, scene management.
*/

class Application {
    std::unique_ptr<Controller> controller;
    std::unique_ptr<Scene> currentScene;
    EventManager eventManager;
public:
    Application();
    virtual void run() = 0;
};
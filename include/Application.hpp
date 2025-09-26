#pragma once

#include <memory>
#include "Controller.hpp"
#include "Scene.hpp"
#include "EventManager.hpp"

/*
    Application
    Abstract class that handles creating the window, passing inputs to the controller, scene management.
*/

template <typename ControllerType>
class Application {
protected:
    ControllerType controller;
    std::unique_ptr<Scene> currentScene;
    EventManager eventManager;

    void coreUpdate() {
        currentScene->update();
        controller.update();
    }

public:
    Application() : controller(eventManager) {}
    virtual void run() = 0;
    
};
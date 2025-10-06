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

    //void changeScene(std::unique_ptr<Scene> newScene) {
        // EventManager.reset()
        //currentScene = newScene;
        // Controller.init(currentScene->fetchObjectPoolRef())
        
    //}

    void runEventRoutines() {
        if (!controller.isBusy()) {
            currentScene->receiveAndRespond();
        }
        controller.receiveAndRespond();
    }

public:
    Application() : controller(eventManager) {}
    virtual void run() = 0;
};
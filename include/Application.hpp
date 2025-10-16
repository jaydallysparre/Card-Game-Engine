#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "Controller.hpp"
#include "Scene.hpp"
#include "EventManager.hpp"
#include "DefaultScene.hpp"

/*
    Application
    Abstract class that handles creating the window, passing inputs to the controller, scene management.
*/

using SceneFactory = std::function<std::unique_ptr<Scene>(EventManager&)>;

template <typename ControllerType>
class Application {
protected:
    ControllerType controller;
    std::unique_ptr<Scene> currentScene;
    std::optional<SceneFactory> pending;
    EventManager eventManager;

    void runEventRoutines() {
        if (!controller.isBusy()) {
            currentScene->run();
        }
        controller.receiveAndRespond();
    }

    void attatchControllerToCurrent() {
        controller.attachPool(currentScene->getControllerView());
    }

public:
    Application() : controller(eventManager) {
        currentScene = std::make_unique<DefaultScene>(eventManager);
        attatchControllerToCurrent();
    }

    void setInitialScene(SceneFactory f) {
        currentScene = f(eventManager);
        attatchControllerToCurrent();
    }

    void requestChangeScene(SceneFactory f) {
        pending = std::move(f);
    }
    virtual void run() = 0;
};
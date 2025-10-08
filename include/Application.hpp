#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "Controller.hpp"
#include "Scene.hpp"
#include "EventManager.hpp"

/*
    Application
    Abstract class that handles creating the window, passing inputs to the controller, scene management.
*/

using SceneFactory = std::function<std::unique_ptr<Scene>(EventManager&)>;

template <typename ControllerType>
class Application {
protected:
        // ControllerType controller;
    std::unique_ptr<Controller> controller_;
    std::unique_ptr<Scene> currentScene;
    std::optional<SceneFactory> pending;
    EventManager& eventManager;

    void runEventRoutines() {
        currentScene->receiveAndRespond();
        controller_.receiveAndRespond();
    }

    void attatchControllerToCurrent() {
        controller_->attachPool(currentScene->getControllerView());
    }

public:
    Application(EventManager& em, std::unique_ptr<Controller> controller) : controller_(std::move(controller)), eventManager(em) {}

    void setInitialScene(SceneFactory f) {
        currentScene = f(eventManager);
        attatchControllerToCurrent();
    }

    void requestChangeScene(SceneFactory f) {
        pending = std::move(f);
    }
    virtual void run() = 0;
};
#pragma once
#include "Scene.hpp"
#include "DefaultState.hpp"

class DefaultScene : public Scene {
public:
    DefaultScene(EventManager& em);
};
#pragma once

#include "ObjectPoolViews.hpp"
#include "SceneObject.hpp"

class Factory {
    ObjectPoolSceneView& view;

    static const std::vector<std::string> SUITS;
    static const std::vector<std::string> RANKS;
public:
    Factory(ObjectPoolSceneView& view) : view(view) {}

    ObjectId createDeck(bool full = true, bool shuffled = false);
    ObjectId createButton(std::string text);
};
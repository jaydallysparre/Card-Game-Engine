#pragma once

#include "ObjectPoolViews.hpp"
#include "SceneObject.hpp"

/*
    Factory
    Helper object for the Scene class, to help with instantiating objects in the object pool.
*/

class Factory {
    ObjectPoolSceneView& view;

    static const std::vector<std::string> SUITS;
    static const std::vector<std::string> RANKS;
public:
    Factory(ObjectPoolSceneView& view) : view(view) {}

    ObjectId createDeck(bool full = true, bool shuffled = false);
    ObjectId createHand();
    ObjectId createButton(std::string text);
};
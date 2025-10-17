#pragma once

#include "ObjectPoolViews.hpp"
#include "SceneObject.hpp"
#include <optional>
#include <unordered_set>

/*
    Factory
    Helper object for the Scene class, to help with instantiating objects in the object pool.
*/

class Factory {
    ObjectPoolSceneView& view;

    static const std::unordered_set<std::string> SUITS;
    static const std::unordered_set<std::string> RANKS;
public:
    Factory(ObjectPoolSceneView& view) : view(view) {}

    ObjectId createDeck(bool full = true, bool shuffled = false);
    std::optional<ObjectId> createCard(std::string suit, std::string rank, ObjectId toDeckID);
    ObjectId createHand();
    ObjectId createButton(std::string text);
};
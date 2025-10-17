#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "PoolObject.hpp"
#include "SceneObject.hpp"

//Add tags (use constexpr as it will compute at compile step)
constexpr uint32_t TAG_USABLE = 1u << 0;
constexpr uint32_t TAG_GRABBABLE = 1u << 1;
constexpr uint32_t TAG_RECEIVABLE = 1u << 2;
constexpr uint32_t TAG_RENDERABLE = 1u << 3;

class ObjectPool {
private:
    // Store parents ID of a card
    std::unordered_map<int, int> parentsMap;
    // Store objects
    std::vector<std::unique_ptr<PoolObject>> objects;
    ObjectId nextId{1};
    // Current active player
    ObjectId activePlayer{0};
    static inline bool hasAny(uint32_t mask, uint32_t test) { return (mask & test) != 0; }
    static inline bool hasAll(uint32_t mask, uint32_t test) { return (mask & test) == test; }
public:
    // Add object to pool
    ObjectId add(std::unique_ptr<PoolObject> obj);
    //Get object by id
    PoolObject* getPointer(ObjectId id);
    // Add tag to object
    void addTag(ObjectId id, uint32_t tag);
    // Rm object tag
    void rmTag(ObjectId id, uint32_t tag);
    // Set active player
    void setActivePlayer(ObjectId playerId);
    // Set player hand
    void setPlayerHand(ObjectId playerId, ObjectId handId);
    // Add score
    void addToScore(ObjectId playerId, ObjectId point);
    // Return the active player (current's player turn)
    ObjectId returnActivePlayer();
    // Return hand id of current player
    ObjectId currentPlayerHandId();
    // Set parent's id for card (which deck owns that card)
    void setParent(ObjectId id, ObjectId parentId);
    // Get card's parent id
    ObjectId getParent(ObjectId id);
    //check if match some of the tags
    bool hasAnyTag(ObjectId id, uint32_t tag) const;
    //check if match all tags
    bool hasAllTags(ObjectId id, uint32_t tag) const;
    //return type
    std::vector<ObjectId> ofType(ObjType type) const;
    //return any tag match
    std::vector<ObjectId> withTags(uint32_t tag) const;
    //return all tag match
    std::vector<ObjectId> withAllTags(uint32_t tagAll) const;
    // return the deck that can grabbable
    std::vector<ObjectId> grabbableDeck() const;
    // return the deck that can receivable
    std::vector<ObjectId> receivableDeck() const;
    // remove from pool
    void remove(ObjectId id);
};
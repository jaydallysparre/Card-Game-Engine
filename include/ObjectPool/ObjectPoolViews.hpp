#pragma once
#include "ObjectPool.hpp"

// Decide which side (Scene, Controller) can do something
struct ObjectPoolSceneView {
    ObjectPool* p{};
    ObjectId add(std::unique_ptr<PoolObject> o) { return p->add(std::move(o)); }
    PoolObject* getPointer(ObjectId id) { return p->getPointer(id); } 
    void addTag(ObjectId id, uint32_t tag) { return p->addTag(id, tag); }
    void rmTag(ObjectId id, uint32_t tag) { return p->rmTag(id, tag); }
    auto ofType(ObjType type) const { return p->ofType(type); }
    auto withTags(uint32_t tags) const { return p->withTags(tags); }
    auto withAllTags(uint32_t tags) const { return p->withAllTags(tags); }
    auto grabbableDeck() const { return p->grabbableDeck(); }
    auto receivableDeck() const { return p->receivableDeck(); }
    auto setParent(ObjectId id, ObjectId parentId) { return p->setParent(id, parentId); }
    auto getParent(ObjectId id) { return p->getParent(id); }
    auto setActivePlayer(ObjectId playerId) { return p->setActivePlayer(playerId); }
    auto setPlayerHand(ObjectId playerId, ObjectId handId) { return p->setPlayerHand(playerId, handId); }
    auto addToScore(ObjectId playerId, ObjectId point) { return p->addToScore(playerId, point); }
    auto returnActivePlayer() { return p->returnActivePlayer(); }
    auto currentPlayerHandId() { return p->currentPlayerHandId(); }
    void remove(ObjectId id) { p->remove(id); }
};

struct ObjectPoolControllerView {
    const ObjectPool* p{};
    const PoolObject* getPointer(ObjectId id) const { return const_cast<ObjectPool*>(p)->getPointer(id); }
    auto ofType(ObjType type) const { return p->ofType(type); }
    auto withTags(uint32_t tags) const { return p->withTags(tags); }
    auto withAllTags(uint32_t tags) const { return p->withAllTags(tags); }
    auto grabbableDeck() const { return p->grabbableDeck(); }
    auto receivableDeck() const { return p->receivableDeck(); }
    auto getParent(ObjectId id) { return const_cast<ObjectPool*>(p)->getParent(id); }
    auto returnActivePlayer() { return const_cast<ObjectPool*>(p)->returnActivePlayer(); }
    auto currentPlayerHandId() { return const_cast<ObjectPool*>(p)->currentPlayerHandId(); }
};
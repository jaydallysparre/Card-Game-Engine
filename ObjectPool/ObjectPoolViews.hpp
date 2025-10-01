#pragma once
#include "ObjectPool.hpp"

struct ObjectPoolSceneView {
    ObjectPool* p{};
    ObjectId add(std::unique_ptr<PoolObject> o) { return p->add(std::move(o)); }
    PoolObject* getPointer(ObjectId id) { return p->getPointer(id); } 
    void setEnable(ObjectId id, bool on) { return p->setEnable(id, on); }
    void addTag(ObjectId id, uint32_t tag) { return p->addTag(id, tag); }
    void rmTag(ObjectId id, uint32_t tag) { return p->rmTag(id, tag); }
    auto ofType(ObjType type) const { return p->ofType(type); }
    auto withTags(uint32_t tags) const { return p->withTags(tags); }
    auto withAllTags(uint32_t tags) const { return p->withAllTags(tags); }
    auto grabbableDeck() const { return p->grabbableDeck(); }
    auto receivableDeck() const { return p->receivableDeck(); }
};

struct ObjectPoolControllerView {
    const ObjectPool* p{};
    const PoolObject* getPointer(ObjectId id) const { return const_cast<ObjectPool*>(p)->getPointer(id); }
    auto ofType(ObjType type) const { return p->ofType(type); }
    auto withTags(uint32_t tags) const { return p->withTags(tags); }
    auto withAllTags(uint32_t tags) const { return p->withAllTags(tags); }
    auto grabbableDeck() const { return p->grabbableDeck(); }
    auto receivableDeck() const { return p->receivableDeck(); }
};
#include "ObjectPool.hpp"

ObjectId ObjectPool::add(std::unique_ptr<PoolObject> obj) {
    obj->id = nextId++;
    objects.push_back(std::move(obj));
    return objects.back()->id;
}

PoolObject* ObjectPool::getPointer(ObjectId id) {
    for (auto& p : objects) {
        if (p && p->id == id) return p.get();
    }
    return nullptr;
}

void ObjectPool::addTag(ObjectId id, uint32_t tag) { if (auto* o = getPointer(id)) o->tags |= tag; }
void ObjectPool::rmTag(ObjectId id, uint32_t tag) { if (auto* o = getPointer(id)) o->tags &= ~tag; }
void ObjectPool::setActivePlayer(ObjectId playerId) { activePlayer = playerId; }

void ObjectPool::setPlayerHand(ObjectId playerId, ObjectId handId) {
    if (auto* p = getPointer(playerId)) if (p->type() == ObjType::Player) {
        static_cast<Player*>(p)->hand = handId;
    }
}

void ObjectPool::addToScore(ObjectId playerId, ObjectId point) {
    if (auto* p = getPointer(playerId)) if (p->type() == ObjType::Player) {
        static_cast<Player*>(p)->score += point;
    }
}

ObjectId ObjectPool::returnActivePlayer() { return activePlayer; }

ObjectId ObjectPool::currentPlayerHandId() {
    auto* p = getPointer(activePlayer);
    if (!p || p->type() != ObjType::Player) return 0;
    auto* pl = static_cast<Player*>(p);
    return pl->hand;
}

void ObjectPool::setParent(ObjectId id, ObjectId parentId) { parentsMap[id] = parentId; }
ObjectId ObjectPool::getParent(ObjectId id) { return parentsMap[id]; }

bool ObjectPool::hasAnyTag(ObjectId id, uint32_t tag) const {
    if (auto* o = const_cast<ObjectPool*>(this)->getPointer(id)) { return hasAny(o->tags, tag); }
    return false;
}

bool ObjectPool::hasAllTags(ObjectId id, uint32_t tag) const {
    if (auto* o = const_cast<ObjectPool*>(this)->getPointer(id)) { return hasAll(o->tags, tag); }
    return false;
}

std::vector<ObjectId> ObjectPool::ofType(ObjType type) const {
    std::vector<ObjectId> out;
    for (auto& p : objects) {
        if (p && p->enabled && (p->type() == type)) out.push_back(p->id);
    }
    return out;
}

std::vector<ObjectId> ObjectPool::withTags(uint32_t tag) const {
    std::vector<ObjectId> out;
    for (auto& p : objects) {
        if (p && p->enabled && (p->tags & tag)) out.push_back(p->id);
    }
    return out;
}

std::vector<ObjectId> ObjectPool::withAllTags(uint32_t tagAll) const {
    std::vector<ObjectId> out;
    for (auto& p : objects)
        if (p && p->enabled && ((p->tags & tagAll) == tagAll)) out.push_back(p->id);
    return out;
}

std::vector<ObjectId> ObjectPool::grabbableDeck() const {
    std::vector<ObjectId> out;
    for (auto& p : objects) {
        if (p && p->enabled && (p->type() == ObjType::Deck) && (p->tags & TAG_GRABBABLE)) out.push_back(p->id);
    }
    return out;
}

std::vector<ObjectId> ObjectPool::receivableDeck() const {
    std::vector<ObjectId> out;
    for (auto& p : objects) {
        if (p && p->enabled && (p->type() == ObjType::Deck) && (p->tags & TAG_RECEIVABLE)) out.push_back(p->id);
    }
    return out;
}
#pragma once
#include <cstdint>
#include <memory>
#include <vector>

using ObjectId = uint32_t;

enum class ObjType : uint8_t {Card, Deck, Hand, Button, PositionalString, Menu, Player, Text};

//Add tags (use constexpr as it will compute at compile step)
constexpr uint32_t TAG_USABLE = 1u << 0;
constexpr uint32_t TAG_GRABBABLE = 1u << 1;
constexpr uint32_t TAG_RECEIVABLE = 1u << 2;
constexpr uint32_t TAG_RENDERABLE = 1u << 3;

struct PoolObject {
    virtual ~PoolObject() = default;
    uint32_t tags{0};
    ObjectId id{0};
    bool enabled{true};
    virtual ObjType type() const = 0;
};

class ObjectPool {
private:
    std::vector<std::unique_ptr<PoolObject>> objects;
    ObjectId nextId{1};
    ObjectId activePlayer{0};

    static inline bool hasAny(uint32_t mask, uint32_t test) { return (mask & test) != 0; }
    static inline bool hasAll(uint32_t mask, uint32_t test) { return (mask & test) == test; }
public:
    ObjectId add(std::unique_ptr<PoolObject> obj) {
        obj->id = nextId++;
        objects.push_back(std::move(obj));
        return objects.back()->id;
    }
    
    //Get object by id
    PoolObject* getPointer(ObjectId id) {
        for (auto& p : objects) {
            if (p && p->id == id) return p.get();
        }
        return nullptr;
    }

    void setEnable(ObjectId id, bool on) { if (auto* o = getPointer(id)) o->enabled = on; };
    void addTag(ObjectId id, uint32_t tag) { if (auto* o = getPointer(id)) o->tags |= tag; };
    void rmTag(ObjectId id, uint32_t tag) { if (auto* o = getPointer(id)) o->tags &= ~tag; };
    
    /*
    void setActivePlayer(ObjectId playerId) { activePlayer = playerId; }
    void setPlayerHand(ObjectId playerId, ObjectId handId) {
        if (auto* p = getPointer(playerId)) if (p->type()==ObjType::Player) {
            static_cast<Player*>(p)->hand = handId;
        }
    }

    void addToScore(ObjectId playerId, ObjectId point) {
        if (auto* p = getPointer(playerId)) if (p->type()==ObjType::Player) {
            static_cast<Player*>(p)->score += point;
        }
    }

    ObjectId activePlayer() const { return activePlayer; }

    ObjectId currentPlayerHandId() { 
        auto* p = getPointer(activePlayer);
        if (!p || p->type() != ObjType::Player) return 0;
        auto* pl = static_cast<Player*>(p);
        return pl->hand; 
    }*/

    //check if match some of the tags
    bool hasAnyTag(ObjectId id, uint32_t tag) const {
        if (auto* o = const_cast<ObjectPool*>(this)->getPointer(id)) {
            return hasAny(o->tags, tag);
        }
        return false;
    }
    //check if match all tags
    bool hasAllTags(ObjectId id, uint32_t tag) const {
        if (auto* o = const_cast<ObjectPool*>(this)->getPointer(id)) {
            return hasAll(o->tags, tag);
        }
        return false;
    }

    //return type
    std::vector<ObjectId> ofType(ObjType type) const {
        std::vector<ObjectId> out;
        for (auto& p : objects) {
            if (p && p->enabled && (p->type() == type)) out.push_back(p->id);
        }
        return out;
    }

    //return any tag match
    std::vector<ObjectId> withTags(uint32_t tag) const {
        std::vector<ObjectId> out;
        for (auto& p : objects) {
            if (p && p->enabled && (p->tags & tag)) out.push_back(p->id);
        }
        return out;
    }

    //return all tag match
    std::vector<ObjectId> withAllTags(uint32_t tagAll) const {
        std::vector<ObjectId> out;
        for (auto& p : objects)
            if (p && p->enabled && ((p->tags & tagAll) == tagAll)) out.push_back(p->id);
        return out;
    }

    std::vector<ObjectId> grabbableDeck() const {
        std::vector<ObjectId> out;
        for (auto& p : objects) {
            if (p && p->enabled && (p->type() == ObjType::Deck) && (p->tags & TAG_GRABBABLE)) out.push_back(p->id);
        }
        return out;
    }

    std::vector<ObjectId> receivableDeck() const {
        std::vector<ObjectId> out;
        for (auto& p : objects) {
            if (p && p->enabled && (p->type() == ObjType::Deck) && (p->tags & TAG_RECEIVABLE)) out.push_back(p->id);
        }
        return out;
    }
};
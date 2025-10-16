#pragma once
#include <cstdint>
#include <memory>
#include <vector>

using ObjectId = uint32_t;

enum class ObjType : uint8_t {Card, Deck, Hand, Button, PositionalString, Menu, Player, Text};
// Struct for object
struct PoolObject {
    virtual ~PoolObject() = default;
    uint32_t tags{0};
    ObjectId id{0};
    bool enabled{true};
    virtual ObjType type() const = 0;
};
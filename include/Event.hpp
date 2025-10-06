#pragma once

/*
    Event structs, store data regarding type of event, and other required data. Authoritative Events are data on the scene end;
    the scene controls the object pool directly. Request events are what the controller sends to the scene, to ask it to perform
    certain actions.
*/

enum class AuthEvent {
    MovedCard, 
    CreatedObject, 
    DisabledObject, 
    MovedObject,
    Shuffled, 
    ToggledObject, 
    FlippedCard
};
enum class ReqEvent {
    MoveCard, 
    PressButton, 
    FlipCard
};

/*
    Base event structs.
*/

struct AuthoritativeEvent {
    AuthEvent eventType;
    AuthoritativeEvent(AuthEvent e) : eventType(e) {}
};

struct RequestEvent {
    ReqEvent eventType;
    RequestEvent(ReqEvent e) : eventType(e) {}
};

/*
    Authoritative Events
*/

struct MovedCard : AuthoritativeEvent {
    const int cardID;
    const int fromID;
    const int toID;

    MovedCard(int cardID, int fromID, int toID) : AuthoritativeEvent(AuthEvent::MovedCard),
    cardID(cardID), fromID(fromID), toID(toID) {}
};

struct CreatedObject : AuthoritativeEvent {
    const int ID;
    const double x;
    const double y;

    CreatedObject(int ID) : AuthoritativeEvent(AuthEvent::CreatedObject), ID(ID), x(x), y(y) {}
};

struct DisabledObject : AuthoritativeEvent {
    const int ID;

    DisabledObject(int ID) : AuthoritativeEvent(AuthEvent::DisabledObject), ID(ID) {}
};

struct MovedObject : AuthoritativeEvent {
    const int ID;
    const double x;
    const double y;

    MovedObject(int ID, double x, double y) : AuthoritativeEvent(AuthEvent::MovedObject), ID(ID), x(x), y(y) {}
};

struct Shuffled : AuthoritativeEvent {
    const int ID;

    Shuffled(int ID) : AuthoritativeEvent(AuthEvent::Shuffled), ID(ID) {}
};

// For toggling objects visibility AND functionality; useful for menus, buttons
struct ToggledObject : AuthoritativeEvent {
    const int ID;

    ToggledObject(int ID) : AuthoritativeEvent(AuthEvent::ToggledObject), ID(ID) {}
};

struct FlippedCard : AuthoritativeEvent {
    const int ID;

    FlippedCard(int ID) :AuthoritativeEvent(AuthEvent::FlippedCard), ID(ID) {}
};

/*
    Request Events
*/
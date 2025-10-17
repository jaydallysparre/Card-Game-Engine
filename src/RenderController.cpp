#include "RenderController.hpp"
#include "SceneObject.hpp"

RenderController::RenderController(EventManager& em) : Controller(em) {}

void RenderController::init(ObjectPoolControllerView objPool) {
    Controller::init(objPool);
    // reset position handler
    positionHandler = RenderPosition();
}

void RenderController::createdObject(int ID, double x, double y) {
    auto obj = poolView_.getPointer(ID);

    // if object is a deck, then register all of it's cards to the same position
    if (obj->type() == ObjType::Deck) {
        const Deck* deck = static_cast<const Deck*>(obj);

        for (ObjectId cardId: deck->getCards()) {
            positionHandler.registerObjectPos(cardId, x, y, deck->id);
        }
    } else if (obj->type() == ObjType::Hand) {
        const Hand* hand = static_cast<const Hand*>(obj);
        
        for (ObjectId cardId: hand->getCards()) {
            positionHandler.registerObjectPos(cardId, x, y, hand->id);
        }
    }
    positionHandler.registerObjectPos(ID, x, y);
}

void RenderController::movedObject(int ID, double x, double y) {
    positionHandler.setWishPos(ID, x, y, 2.5);
}
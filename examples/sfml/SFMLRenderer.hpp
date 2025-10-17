#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include "RenderPosition.hpp"
#include "SceneObject.hpp"
#include "ObjectPoolViews.hpp"

class SFMLRenderer {
    // text constants
    const int FONT_SIZE = 16;
    const int STATUS_SIZE = 12;
    const float STATUS_DURATION = 2.0f;
    // button constants
    const sf::Color BTN_COLOUR = sf::Color(0, 0, 0, 100);
    const float BTN_MARGIN_X = 10.0f;
    const float BTN_MARGIN_Y = 5.0f;
    // card constants
    const float CARD_WIDTH_DEF = 96.0f;
    const float CARD_HEIGHT_DEF = 128.0f;
    // current card sizes
    float CARD_WIDTH = CARD_WIDTH_DEF;
    float CARD_HEIGHT = CARD_HEIGHT_DEF;
    // load textures into textureMap to save creating textures every frame
    std::unordered_map<std::string, sf::Texture> textureMap;
    // font
    sf::Font font;
    sf::Text statusMsg;
    sf::Clock statusTimer;
    // path to card images
    const std::string path = "examples/sfml/card-png/";
    // vectors used to generate card combinations at instantiation
    const std::vector<std::string> SUITS = {"H", "D", "C","S"};
    const std::vector<std::string> RANKS = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    // deck maps
    std::unordered_map<int, sf::FloatRect> deckBounds;
    std::unordered_map<int, sf::Text> deckLabels;
    // hand maps
    std::unordered_map<int, sf::FloatRect> handBounds;
    // button maps
    std::unordered_map<int, sf::FloatRect> btnBounds;
    std::unordered_map<int, sf::Text> btnLabels;
    RenderPosition& positionHandler;
    ObjectPoolControllerView& view;
    // Function to combine rectangles, so that if we have card offsets hitbox is accurate
    sf::FloatRect combineRect(const sf::FloatRect& rect1, const sf::FloatRect& rect2);
    void renderDropShadow(sf::RenderWindow& window, const sf::Sprite& sprite) const;
public:
    SFMLRenderer(RenderPosition& renderPos, ObjectPoolControllerView& view);
    void renderDeck(sf::RenderWindow& window, const Deck* deck, int ID, std::optional<ObjectId> ignoreCardId = std::nullopt);
    void renderHand(sf::RenderWindow& window, const Hand* hand, int ID, std::optional<ObjectId> ignoreCardId = std::nullopt);
    void renderDeckLabel(sf::RenderWindow& window, int ID);
    sf::FloatRect renderCard(sf::RenderWindow& window, ObjectId cardId);
    void renderButton(sf::RenderWindow& window, ObjectId ID);
    std::optional<ObjectId> getDeckAtPos(sf::RenderWindow& window, sf::Vector2i pos, ObjectId ignoreID=UINT32_MAX);
    sf::FloatRect getCardBounds(sf::RenderWindow& window, ObjectId cardId);
    std::optional<ObjectId> getHandAtPos(sf::RenderWindow& window, sf::Vector2i pos);
    std::optional<ObjectId> getTopCardAtPosInHand(sf::RenderWindow& window, sf::Vector2i pos, int handID);
    std::optional<ObjectId> getTopCardAtPos(sf::RenderWindow& window, sf::Vector2i pos);
    std::optional<ObjectId> getButtonAtPos(sf::RenderWindow& window, sf::Vector2i pos);
    void setDeckLabel(int ID, const std::string& str);
    void createBtn(ObjectId ID);
    void setStatus(std::string& status);
    void renderStatus(sf::RenderWindow& window, float dt);
    void calcNewSizes(sf::RenderWindow& window);
};
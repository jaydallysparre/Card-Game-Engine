#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include "RenderPosition.hpp"
#include "SceneObject.hpp"

class SFMLRenderer {
    // text constants
    const int FONT_SIZE = 16;
    const int STATUS_SIZE = 12;
    const float STATUS_DURATION = 2.0f;

    // current card sizes
    float CARD_WIDTH = 96.0f;
    float CARD_HEIGHT = 128.0f;

    const float CARD_WIDTH_RATIO = CARD_WIDTH/800.0f;
    const float CARD_ASPECT_RATIO = CARD_WIDTH / CARD_HEIGHT;

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

    RenderPosition& positionHandler;
    ObjectPoolControllerView& view;

    // Function to combine rectangles, so that if we have card offsets hitbox is accurate
    sf::FloatRect combineRect(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
        // get bounding box sides of both rectangles
        float left = std::min(rect1.left, rect2.left);
        float right = std::max(rect1.left + rect1.width, rect2.left + rect2.width);
        float top = std::min(rect1.top, rect2.top);
        float bot = std::max(rect1.top + rect1.height, rect2.top + rect2.height);

        return sf::FloatRect(left, top, right - left, bot - top);
    }
public:
    SFMLRenderer(RenderPosition& renderPos, ObjectPoolControllerView& view) : positionHandler(renderPos), view(view) {
        // generate file path names at runtime for our card-png examples.
        for (const std::string& s : SUITS) {
            for (const std::string& r : RANKS) {
                std::string key = r + s; 
                std::string filepath = path + key + ".png";
            
                if (!textureMap[key].loadFromFile(filepath)) {
                    std::cerr << "Failed to load texture: " << filepath << '\n';
                }

                textureMap[key].setSmooth(true);
            }
        }

        // empty texture
        if (!textureMap["empty"].loadFromFile(path + "empty.png")) {
            std::cerr << "Failed to load empty texture\n";
        }

        if (!textureMap["hand_placeholder"].loadFromFile(path + "hand_placeholder.png")) {
            std::cerr << "Failed to load hand placeholder\n";
        }

        textureMap["empty"].setSmooth(true);
        textureMap["hand_placeholder"].setSmooth(true);

        // load font
        if (!font.loadFromFile("examples/sfml/font/inter.ttf")) {
            std::cerr << "Failed to load font\n";
        }
    }

    void renderDeck(sf::RenderWindow& window, const Deck* deck, int ID, std::optional<ObjectId> ignoreCardId = std::nullopt) {
        // if we have a label for this deck, render it first
        if (deckLabels.count(ID)) {
            renderDeckLabel(window, ID);
        }

        auto topCards = deck->top2Cards();    

        sf::FloatRect deckBox;
        sf::FloatRect tempRect;

        // render the second card below the top card.
        if (topCards.second) {
            deckBox = renderCard(window, *topCards.second);

        // else render the empty card
        } else {
            auto deckPos = positionHandler.getPos(ID);
            sf::Vector2f sfmlPos = {
                static_cast<float>(deckPos.first) * window.getSize().x,
                static_cast<float>(deckPos.second) * window.getSize().y
            };

            sf::Sprite emptySprite(textureMap["empty"]);
            sf::Vector2u texSize = textureMap["empty"].getSize();
            sf::Vector2f scaledSizes = {CARD_WIDTH/texSize.x, CARD_HEIGHT/texSize.y};
            emptySprite.setScale(scaledSizes);
            emptySprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
            emptySprite.setPosition(sfmlPos);
            
            window.draw(emptySprite);
            
            // Store the empty deck bounds
            deckBox = emptySprite.getGlobalBounds();
        }

        // render the top card in deck, if it exists
        if (topCards.first) {
            if (!ignoreCardId || *ignoreCardId != *topCards.first) {
                tempRect = renderCard(window, *topCards.first);
            
                // combine hitboxes
                deckBox = combineRect(deckBox, tempRect);
            }    
        }

        deckBounds[ID] = deckBox;

    }

    void renderHand(sf::RenderWindow& window, const Hand* hand, int ID, std::optional<ObjectId> ignoreCardId = std::nullopt) {
        const auto& cards = hand->getCards();
        if (cards.empty()) {
            // render an empty placeholder for the hand area
            auto handPos = positionHandler.getPos(ID);
            sf::Vector2f sfmlPos = {
                static_cast<float>(handPos.first) * window.getSize().x,
                static_cast<float>(handPos.second) * window.getSize().y
            };
            sf::Sprite placeholderSprite(textureMap["hand_placeholder"]);
            sf::Vector2u texSize = textureMap["hand_placeholder"].getSize();
            float scale = CARD_WIDTH / texSize.x;
            placeholderSprite.setScale(scale, scale);
            placeholderSprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
            placeholderSprite.setPosition(sfmlPos);
            
            window.draw(placeholderSprite);
            
            // Store its bounds for mouse interaction
            handBounds[ID] = placeholderSprite.getGlobalBounds();
            return;
        }

        sf::FloatRect combinedBounds;
        bool firstCard = true;

        //render card in hands
        for (ObjectId cardId : cards) {
            sf::FloatRect cardBounds = renderCard(window, cardId);
            if (firstCard) {
                combinedBounds = cardBounds;
                firstCard = false;
            } else {
                combinedBounds = combineRect(combinedBounds, cardBounds);
            }
        }

        handBounds[ID] = combinedBounds;
    }

    void renderDeckLabel(sf::RenderWindow& window, int ID) {
        auto deckPos = positionHandler.getPos(ID);

        // convert normalized position to actual position
        sf::Vector2f sfmlPos { static_cast<float>(deckPos.first) * window.getSize().x,
                                static_cast<float>(deckPos.second) * window.getSize().y };
   
        deckLabels[ID].setPosition({sfmlPos.x, sfmlPos.y + CARD_HEIGHT / 2.0f + 1});    
        window.draw(deckLabels[ID]);
    }

    sf::FloatRect renderCard(sf::RenderWindow& window, ObjectId cardId) {
        std::pair<double, double> currPos = positionHandler.getPos(cardId);

        const Card* card = static_cast<const Card*>(view.getPointer(cardId));

        // calculate correct sfml position using normalized coordinates
        sf::Vector2f sfmlPos = {static_cast<float>(currPos.first) * window.getSize().x, 
                                static_cast<float>(currPos.second) * window.getSize().y};

        // get card key for accessing texture
        std::string cardKey = card->getRank() + card->getSuit();

        if (!textureMap.count(cardKey)) {
            std::cerr << "Card texture could not be retrieved from map\n";
            return sf::FloatRect(); 
        }

        sf::Sprite cardSprite(textureMap[cardKey]);
        sf::Vector2u texSize = textureMap[cardKey].getSize();

        // scale render size to CARD_WIDTH, CARD_HEIGHT
        sf::Vector2f scaledSizes = {CARD_WIDTH/texSize.x, CARD_HEIGHT/texSize.y};
        cardSprite.setScale(scaledSizes);
        cardSprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f); // center-based positioning
        cardSprite.setPosition(sfmlPos);

        // basic drop shadow
        sf::Sprite shadowSprite = cardSprite;
        shadowSprite.move(4.0f, 4.0f);
        shadowSprite.setColor(sf::Color(0,0,0,20));
        window.draw(shadowSprite);

        window.draw(cardSprite);

        return cardSprite.getGlobalBounds();
    }

    std::optional<ObjectId> getDeckAtPos(sf::RenderWindow& window, sf::Vector2i pos, ObjectId ignoreID=UINT32_MAX) {
        sf::Vector2f worldPos = window.mapPixelToCoords(pos);

        for (const auto& [ID, rect]: deckBounds) {
            if (rect.contains(worldPos) && ID != ignoreID)
                return ID;
        }

        return std::nullopt;
    }

    sf::FloatRect getCardBounds(sf::RenderWindow& window, ObjectId cardId) {
        auto cardPos = positionHandler.getPos(cardId);
        sf::Vector2f sfmlPos = {
            static_cast<float>(cardPos.first) * window.getSize().x,
            static_cast<float>(cardPos.second) * window.getSize().y
        };

        const Card* card = static_cast<const Card*>(view.getPointer(cardId));
        std::string cardKey = card->getRank() + card->getSuit();
        if (!textureMap.count(cardKey)) return sf::FloatRect();

        sf::Sprite cardSprite(textureMap[cardKey]);
        sf::Vector2u texSize = textureMap[cardKey].getSize();
        sf::Vector2f scaledSizes = {CARD_WIDTH/texSize.x, CARD_HEIGHT/texSize.y};
        cardSprite.setScale(scaledSizes);
        cardSprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
        cardSprite.setPosition(sfmlPos);
        return cardSprite.getGlobalBounds();
    }

    std::optional<ObjectId> getHandAtPos(sf::RenderWindow& window, sf::Vector2i pos) {
        sf::Vector2f worldPos = window.mapPixelToCoords(pos);

        for (const auto& [ID, rect]: handBounds) {
            if (rect.contains(worldPos))
                return ID;
        }

        return std::nullopt;
    }

    std::optional<ObjectId> getTopCardAtPosInHand(sf::RenderWindow& window, sf::Vector2i pos, int handID) {
        const Hand* hand = static_cast<const Hand*>(view.getPointer(handID));
        if (!hand) return std::nullopt;

        sf::Vector2f worldPos = window.mapPixelToCoords(pos);

        const auto& cards = hand->getCards();
        // iterate backwards because the last card is rendered on top.
        for (auto it = cards.rbegin(); it != cards.rend(); ++it) {
            ObjectId cardId = *it;
            sf::FloatRect bounds = getCardBounds(window, cardId);
            if (bounds.contains(worldPos)) {
                return cardId;
            }
        }
        return std::nullopt;
    }

    std::optional<ObjectId> getTopCardAtPos(sf::RenderWindow& window, sf::Vector2i pos) {
        auto deckID = getDeckAtPos(window, pos);

        if (!deckID) return std::nullopt;

        const Deck* deck = static_cast<const Deck*>(view.getPointer(*deckID));

        return deck->topCard();
    }

    void setDeckLabel(int ID, const std::string& str) {
        sf::Text label;
        label.setFont(font);
        label.setString(str);
        label.setCharacterSize(FONT_SIZE);
        label.setOrigin(std::round(label.getLocalBounds().width/2.0f), 0.0f);
        deckLabels[ID] = label;
    }

    void setStatus(std::string& status) {
        statusMsg.setFont(font);
        statusMsg.setString(status);
        statusMsg.setCharacterSize(STATUS_SIZE);
        statusTimer.restart();
    }

    void renderStatus(sf::RenderWindow& window, float dt) {
        statusMsg.setPosition(window.getSize().x - statusMsg.getLocalBounds().width - 5.0f, 5.0f);

        // fade-out text over STATUS_DURATION
        float currTime = statusTimer.getElapsedTime().asSeconds();
        sf::Color textColour = sf::Color::White;
        textColour.a = 255 * std::clamp(1.0f - (currTime/STATUS_DURATION), 0.0f, 1.0f);
        statusMsg.setFillColor(textColour);
        
        window.draw(statusMsg);
    }

    void calcNewCardSize(sf::RenderWindow& window) {
        CARD_WIDTH = window.getSize().x * CARD_WIDTH_RATIO;
        CARD_HEIGHT = CARD_WIDTH / CARD_ASPECT_RATIO;
    }
};
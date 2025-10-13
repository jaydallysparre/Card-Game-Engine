#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include "RenderPosition.hpp"
#include "SceneObject.hpp"

class SFMLRenderer {
    const float CARD_WIDTH = 120.0f;
    const float CARD_HEIGHT = 160.0f;

    // load textures into textureMap to save creating textures every frame
    std::unordered_map<std::string, sf::Texture> textureMap;

    // path to card images
    const std::string path = "examples/sfml/card-png/";

    // vectors used to generate card combinations at instantiation
    const std::vector<std::string> SUITS = {"H", "D", "C","S"};
    const std::vector<std::string> RANKS = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    std::unordered_map<int, sf::FloatRect> cardBounds;
    RenderPosition& positionHandler;
    ObjectPoolControllerView& view;
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
    }

    void renderDeck(sf::RenderWindow& window, const Deck* deck, int ID) {
        auto topCards = deck->top2Cards();

        // render the second card below the top card.
        if (topCards.second)
            renderCard(window, *topCards.second);

        if (topCards.first)
            renderCard(window, *topCards.first);
        
    }

    void renderCard(sf::RenderWindow& window, ObjectId cardId) {
        std::pair<double, double> currPos = positionHandler.getPos(cardId);

        const Card* card = static_cast<const Card*>(view.getPointer(cardId));

        // calculate correct sfml position using normalized coordinates
        sf::Vector2f sfmlPos = {static_cast<float>(currPos.first) * window.getSize().x, 
                                static_cast<float>(currPos.second) * window.getSize().y};

        // get card key for accessing texture
        std::string cardKey = card->getRank() + card->getSuit();

        if (!textureMap.count(cardKey)) {
            std::cerr << "Card texture could not be retrieved from map\n";
            return; 
        }

        sf::Sprite cardSprite(textureMap[cardKey]);
        sf::Vector2u texSize = textureMap[cardKey].getSize();

        // scale render size to CARD_WIDTH, CARD_HEIGHT
        sf::Vector2f scaledSizes = {CARD_WIDTH/texSize.x, CARD_HEIGHT/texSize.y};
        cardSprite.setScale(scaledSizes);
        cardSprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f); // center-based positioning
        cardSprite.setPosition(sfmlPos);

        // register the card bounds for mouse collision detection
        cardBounds[cardId] = cardSprite.getGlobalBounds();

        window.draw(cardSprite);
    }

    std::optional<int> getCardAtPos(sf::RenderWindow& window, sf::Vector2i pos) {
        sf::Vector2f worldPos = window.mapPixelToCoords(pos);

        for (const auto& [ID, rect]: cardBounds) {
            if (rect.contains(worldPos))
                return ID;
        }

        return std::nullopt;
    }

    std::optional<int> getDeckAtPos(sf::RenderWindow& window, sf::Vector2i pos) {
        sf::Vector2f worldPos = window.mapPixelToCoords(pos);
        
        return std::nullopt;
    }
};
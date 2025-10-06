#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "RenderPosition.hpp"
#include "SceneObject.hpp"

class SFMLRenderer {
    const float CARDSCALE = 0.25;

    // load textures into textureMap to save creating textures every frame
    std::unordered_map<std::string, sf::Texture> textureMap;

    // path to card images
    const std::string path = "examples/sfml/card-png/";

    // vectors used to generate card combinations at instantiation
    const std::vector<std::string> SUITS = {"H", "D", "C","S"};
    const std::vector<std::string> RANKS = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    RenderPosition& positionHandler;
public:
    SFMLRenderer(RenderPosition& renderPos) : positionHandler(renderPos) {

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

        if (topCards.first)
            renderCard(window, *topCards.first, (*topCards.first).id);
        
        if (topCards.second)
            renderCard(window, *topCards.second, (*topCards.second).id);
    }

    void renderCard(sf::RenderWindow& window, Card card, int ID) {
        std::pair<double, double> currPos = positionHandler.getPos(ID);
        sf::Vector2f sfmlPos = {static_cast<float>(currPos.first) * window.getSize().x, static_cast<float>(currPos.second) * window.getSize().y};
        std::string cardKey = card.getRank() + card.getSuit();

        if (!textureMap.count(cardKey)) {
            std::cerr << "Card texture could not be retrieved from map\n";
            return; 
        }

        sf::Sprite cardSprite(textureMap[cardKey]);

        sf::Vector2u texSize = textureMap[cardKey].getSize();

        cardSprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f); // center-based positioning
        cardSprite.setScale(0.25, 0.25);
        cardSprite.setPosition(sfmlPos);

        window.draw(cardSprite);
    }
};
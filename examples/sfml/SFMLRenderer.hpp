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

    std::unordered_map<int, sf::FloatRect> deckBounds;
    RenderPosition& positionHandler;
    ObjectPoolControllerView& view;

private:
    // Function to combine rectangles, so that if we have card offsets hitbox is accurate
    sf::FloatRect combineRect(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
        return sf::FloatRect(
            std::min(rect1.left, rect2.left),
            std::min(rect1.top, rect2.top),
            std::max(rect1.left + rect1.width, rect2.left + rect2.width),
            std::max(rect1.top + rect1.height, rect2.top + rect2.height)
        );
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

        textureMap["empty"].setSmooth(true);
    }

    void renderDeck(sf::RenderWindow& window, const Deck* deck, int ID) {
        auto topCards = deck->top2Cards();

        sf::FloatRect deckBox;
        sf::FloatRect tempRect;

        // render the second card below the top card.
        if (topCards.second) {
            deckBox = renderCard(window, *topCards.second);
        }

        if (topCards.first) {
            tempRect = renderCard(window, *topCards.first);
            
            // combine hitboxes if applicable
            if (topCards.second) {
                deckBox = combineRect(deckBox, tempRect);
            } else {
                deckBox = tempRect;
            }
        }
        
        if (!topCards.first) {
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

        deckBounds[ID] = deckBox;
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

        window.draw(cardSprite);

        return cardSprite.getGlobalBounds();
    }

    std::optional<ObjectId> getDeckAtPos(sf::RenderWindow& window, sf::Vector2i pos) {
        sf::Vector2f worldPos = window.mapPixelToCoords(pos);

        for (const auto& [ID, rect]: deckBounds) {
            if (rect.contains(worldPos))
                return ID;
        }

        return std::nullopt;
    }

    std::optional<ObjectId> getTopCardAtPos(sf::RenderWindow& window, sf::Vector2i pos) {
        auto deckID = getDeckAtPos(window, pos);

        if (!deckID) return std::nullopt;

        const Deck* deck = static_cast<const Deck*>(view.getPointer(*deckID));

        return deck->topCard();
    }
};
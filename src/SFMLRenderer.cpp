#include "SFMLRenderer.hpp"
#include <iostream>
#include <algorithm> // for std::min/max
#include <cmath> // for std::round

sf::FloatRect SFMLRenderer::combineRect(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
    // get bounding box sides of both rectangles
    float left = std::min(rect1.left, rect2.left);
    float right = std::max(rect1.left + rect1.width, rect2.left + rect2.width);
    float top = std::min(rect1.top, rect2.top);
    float bot = std::max(rect1.top + rect1.height, rect2.top + rect2.height);
    return sf::FloatRect(left, top, right - left, bot - top);
}

void SFMLRenderer::renderDropShadow(sf::RenderWindow& window, const sf::Sprite& sprite) const {
    // transparent black
    sf::Color colour(0, 0, 0, 20);
    sf::Sprite shadow(sprite);
    shadow.move(6.0f, 6.0f);
    shadow.setColor(colour);
    window.draw(shadow);
}

SFMLRenderer::SFMLRenderer(RenderPosition& renderPos, ObjectPoolControllerView& view) : positionHandler(renderPos), view(view) {
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

void SFMLRenderer::renderDeck(sf::RenderWindow& window, const Deck* deck, int ID, std::optional<ObjectId> ignoreCardId) {
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

void SFMLRenderer::renderHand(sf::RenderWindow& window, const Hand* hand, int ID, std::optional<ObjectId> ignoreCardId) {
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

void SFMLRenderer::renderDeckLabel(sf::RenderWindow& window, int ID) {
    auto deckPos = positionHandler.getPos(ID);
    // convert normalized position to actual position
    sf::Vector2f sfmlPos { static_cast<float>(deckPos.first) * window.getSize().x,
                            static_cast<float>(deckPos.second) * window.getSize().y };

    deckLabels[ID].setPosition({sfmlPos.x, sfmlPos.y + CARD_HEIGHT / 2.0f + 1});    
    window.draw(deckLabels[ID]);
}

sf::FloatRect SFMLRenderer::renderCard(sf::RenderWindow& window, ObjectId cardId) {
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

    renderDropShadow(window, cardSprite);
    window.draw(cardSprite);

    return cardSprite.getGlobalBounds();
}

void SFMLRenderer::renderButton(sf::RenderWindow& window, ObjectId ID) {
    auto btnPos = positionHandler.getPos(ID);

    // convert normalized position to actual position
    sf::Vector2f sfmlPos { static_cast<float>(btnPos.first) * window.getSize().x,
                            static_cast<float>(btnPos.second) * window.getSize().y };
    
    auto labelBounds = btnLabels[ID].getLocalBounds();

    sf::Vector2f btnSize {labelBounds.width + 2 * BTN_MARGIN_X, labelBounds.height + 2 * BTN_MARGIN_Y};
    // set up button body
    sf::RectangleShape body;
    body.setSize(btnSize);
    auto bodyBounds = body.getLocalBounds();
    body.setOrigin(btnSize.x/2.0f, btnSize.y/4);
    body.setPosition(sfmlPos);
    body.setFillColor(BTN_COLOUR);

    btnLabels[ID].setPosition(sfmlPos);

    window.draw(body);
    window.draw(btnLabels[ID]);

    btnBounds[ID] = body.getGlobalBounds();
}

std::optional<ObjectId> SFMLRenderer::getDeckAtPos(sf::RenderWindow& window, sf::Vector2i pos, ObjectId ignoreID) {
    sf::Vector2f worldPos = window.mapPixelToCoords(pos);

    for (const auto& [ID, rect]: deckBounds) {
        if (rect.contains(worldPos) && ID != ignoreID)
            return ID;
    }

    return std::nullopt;
}

sf::FloatRect SFMLRenderer::getCardBounds(sf::RenderWindow& window, ObjectId cardId) {
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

std::optional<ObjectId> SFMLRenderer::getHandAtPos(sf::RenderWindow& window, sf::Vector2i pos) {
    sf::Vector2f worldPos = window.mapPixelToCoords(pos);

    for (const auto& [ID, rect]: handBounds) {
        if (rect.contains(worldPos))
            return ID;
    }

    return std::nullopt;
}

std::optional<ObjectId> SFMLRenderer::getTopCardAtPosInHand(sf::RenderWindow& window, sf::Vector2i pos, int handID) {
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

std::optional<ObjectId> SFMLRenderer::getTopCardAtPos(sf::RenderWindow& window, sf::Vector2i pos) {
    auto deckID = getDeckAtPos(window, pos);

    if (!deckID) return std::nullopt;

    const Deck* deck = static_cast<const Deck*>(view.getPointer(*deckID));

    return deck->topCard();
}

std::optional<ObjectId> SFMLRenderer::getButtonAtPos(sf::RenderWindow& window, sf::Vector2i pos) {
    sf::Vector2f worldPos = window.mapPixelToCoords(pos);

    for (const auto& [ID, rect]: btnBounds) {
        if (rect.contains(worldPos))
            return ID;
    }

    return std::nullopt;       
}

void SFMLRenderer::setDeckLabel(int ID, const std::string& str) {
    sf::Text label;
    label.setFont(font);
    label.setString(str);
    label.setCharacterSize(FONT_SIZE);
    label.setOrigin(std::round(label.getLocalBounds().width/2.0f), 0.0f);
    deckLabels[ID] = label;
}

void SFMLRenderer::createBtn(ObjectId ID) {
    const Button* btn = static_cast<const Button*>(view.getPointer(ID));
    sf::Text label;
    label.setFont(font);
    label.setCharacterSize(FONT_SIZE);
    label.setString(btn->text);
    label.setOrigin(std::round(label.getLocalBounds().width/2.0f),
                    std::round(label.getLocalBounds().height/2.0f));
    btnLabels[ID] = label;
}

void SFMLRenderer::setStatus(std::string& status) {
    statusMsg.setFont(font);
    statusMsg.setString(status);
    statusMsg.setCharacterSize(STATUS_SIZE);
    statusTimer.restart();
}

void SFMLRenderer::renderStatus(sf::RenderWindow& window, float dt) {
    statusMsg.setPosition(window.getSize().x - statusMsg.getLocalBounds().width - 5.0f, 5.0f);

    // fade-out text over STATUS_DURATION
    float currTime = statusTimer.getElapsedTime().asSeconds();
    sf::Color textColour = sf::Color::White;
    textColour.a = 255 * std::clamp(1.0f - (currTime/STATUS_DURATION), 0.0f, 1.0f);
    statusMsg.setFillColor(textColour);
    
    window.draw(statusMsg);
}

void SFMLRenderer::calcNewSizes(sf::RenderWindow& window) {
    //calculate scale factor based on smaller of either dimension
    float widthScale = static_cast<float>(window.getSize().x) / 800.0f;
    float heightScale = static_cast<float>(window.getSize().y) / 600.0f;
    float uniformScale = std::min(widthScale, heightScale);
    
    // scale accordingly
    CARD_WIDTH = CARD_WIDTH_DEF * uniformScale;
    CARD_HEIGHT = CARD_WIDTH / (CARD_WIDTH_DEF/CARD_HEIGHT_DEF);
}
#pragma once

#include <SFML/Graphics.hpp>
#include "Application.hpp"
#include "SFMLController.hpp"

class SFMLApplication : public Application<SFMLController> {
private:
    sf::RenderWindow window;
    sf::Clock deltaClock;
    float dt;
public:
    SFMLApplication() : window(sf::RenderWindow(sf::VideoMode(800, 600), "Card Game Engine")) {}
    void run() override;
    void handleEvents(sf::RenderWindow& window, sf::Event& event);
};
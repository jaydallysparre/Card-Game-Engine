#pragma once

#include <SFML/Graphics.hpp>
#include "Application.hpp"
#include "SFMLController.hpp"

template <class SpecificScene>
class SFMLApplication : public Application<SFMLController, SpecificScene> {
private:
    sf::RenderWindow window;
    sf::Clock deltaClock;
    float dt;
public:
    SFMLApplication() : window(sf::RenderWindow(sf::VideoMode(800, 600), "Card Game Engine")) {}
    void run() override {
        while (window.isOpen()) {
            // deltaTime for framerate independent rendering
            dt = deltaClock.restart().asSeconds();

            // handle SFML events
            sf::Event event;
            this->handleEvents(window, event);

            // update core components; scene, then controller
            this->runEventRoutines();

            // rendering
            window.clear(sf::Color(44, 99, 47, 255));
            this->controller.render(window, dt);
            
            // update t factor with dt
            if (!this->controller.isBusy()) {
                auto dtEv = std::make_unique<UpdateTFactor>(dt);
                this->eventManager.pushReqEvent(std::move(dtEv));
            }

            window.display();
        }
    }
    void handleEvents(sf::RenderWindow& window, sf::Event& event) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height)));
            }

            // send to controller; will create game events from inputs
            this->controller.handleSFMLEvent(window, event, dt);
        }
    }
};
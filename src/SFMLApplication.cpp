#include "SFMLApplication.hpp"

void SFMLApplication::run() {
    while (window.isOpen()) {
        // deltaTime for framerate independent rendering
        dt = deltaClock.restart().asSeconds();

        // handle SFML events
        sf::Event event;
        handleEvents(window, event);

        // update core components; scene, then controller
        runEventRoutines();

        // rendering
        window.clear(sf::Color(44, 99, 47, 255));
        controller.render(window, dt);
        
        // update t factor with dt
        if (!controller.isBusy()) {
            auto dtEv = std::make_unique<UpdateTFactor>(dt);
            eventManager.pushReqEvent(std::move(dtEv));
        }

        window.display();
    }
}

void SFMLApplication::handleEvents(sf::RenderWindow& window, sf::Event& event) {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::Resized) {
            window.setView(sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height)));
        }

        // send to controller; will create game events from inputs
        controller.handleSFMLEvent(window, event, dt);
    }
}
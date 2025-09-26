#include "SFMLApplication.hpp"

void SFMLApplication::run() {
    while (window.isOpen()) {
        // deltaTime for framerate independent rendering
        dt = deltaClock.restart().asSeconds();

        // handle SFML events
        sf::Event event;
        handleEvents(event);

        // update core components; scene, then controller
        coreUpdate();

        // rendering
        window.clear();
        controller.render(window, dt);
        window.display();
    }
}

void SFMLApplication::handleEvents(sf::Event& event) {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // send to controller; will create game events from inputs
        controller.handleEvent(event, dt);
    }
}
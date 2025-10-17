# Card Game Engine
This is a modular game engine built in C++ for representing classic card games. The engine is designed with flexibility in mind, allowing developers to implement any frontend they desire, or even run games in a headless environment for testing and simulation.

By default, the engine has an example SFML-based frontend.

# Features

- **Modular Design:** The engine architecture uses a split controller/game design:
  - The **game** is the authoritative owner of the game space and its objects.
  - The **controller** makes requests to modify the game state.
  - This separation allows any number of frontends to be written without affecting core game logic.

- **State Machine Games:** Games are represented using a `Scene` class (representing the global game space) and multiple `State` classes to manage game flow.

- **Event-Driven Architecture:** Two types of events facilitate communication between the current scene and the controller:
  - **Authoritative events:** Statements about the game space, sent from the scene to the controller.
  - **Request events:** Requests sent from the controller to the scene to perform an action.
    - Example: When a player drags a card to a deck, the controller may request that the scene moves the card from Deck A to Deck B. The scene can then decide whether to accept or reject the request.
  - This skeptical architecture ensures the controller cannot directly modify the game state, making frontend implementations safer.

# Installation (SFML build)
## Linux
1. ensure you have SFML of the 2.x version series installed on your system (SFML's latest version is 3.x).
2. Build using the included makefile; at the time of writing we have two targets, `sfml` and `adder`, which create executables for the default scene showcase and the binary adder showcase, respectively.

#pragma once

#include "Scene.hpp"
#include "SceneObject.hpp"
#include "EventManager.hpp"
#include "HiloStates.hpp"

class HiLoScene: public Scene{
  // Variable to record the button that was pressed
  public:
  int playerScore = 0; 
  HiLoScene(EventManage& em) : Scene(em) {
    factories["SETUP"] = [&](){
      return std::unique_ptr<State>(std::make_unique<SetupState>(*this, em));
    };
    factories["INPUT"] = [&](){
      return std::unique_ptr<State>(std::make_unique<PlayerInputState>(*this, em));
    };
    factories["CALCULATE"] = [&](){
      return std::unique_ptr<State>(std::make_unique<CalculateScore>(*this, em));
    };
    factories["END"] = [&](){
      return std::unique_ptr<State>(std::make_unique<GameEndState>(*this, em));
    };

    // Create objects
    int deck0Id = objFactory.createDeck(true, true);
    int deck1Id = objFactory.createDeck(false, false);
    int deck2Id = objFactory.createDeck(false, false);
    int button0Id = objFactory.createButton();
    int button1Id = objFactory.createButton();

    // Push the decks
    em.pushAuthEvent(std::make_unique<CreatedObject>(deck0Id, 0.25, 0.3));
    em.pushAuthEvent(std::make_unique<CreatedObject>(deck1Id, 0.5, 0.3));
    em.pushAuthEvent(std::make_unique<CreatedObject>(deck2Id, 0.75, 0.3));

    // Add labels
    em.pushAuthEvent(std::make_unique<SetDescriptor>(deck0Id, "Deck 0"));
    em.pushAuthEvent(std::make_unique<SetDescriptor>(deck1Id, "Deck 1"));
    em.pushAuthEvent(std::make_unique<SetDescriptor>(deck2Id, "Deck 2"));

    // Push buttons
    em.pushAuthEvent(std::make_unique<CreatedObject>(button0Id, 0.35, 0.6));
    em.pushAuthEvent(std::make_unique<CreatedObject>(button1Id, 0.65, 0.6));

    // Add labels
    em.pushAuthEvent(std::make_unique<SetDescriptor>(button0Id, "High"));
    em.pushAuthEvent(std::make_unique<SetDescriptor>(button1Id, "Low"));
  }

};


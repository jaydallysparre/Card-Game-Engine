#include <SFML/Graphics.hpp>
#include "SFMLApplication.hpp"
#include "HiLoScene.hpp"

int main() {
    SFMLApplication sfmlApp;
    setInitialScene([](EventManager& em){std::make_unique<HiLoScene>});
    sfmlApp.setInitialScene(HiLoScene);
    sfmlApp.run();
}
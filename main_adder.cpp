#include <SFML/Graphics.hpp>
#include "SFMLApplication.hpp"
#include "AdderScene.hpp"

int main() {
    SFMLApplication<AdderScene> sfmlApp;
    sfmlApp.run();
}
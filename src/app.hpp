#pragma once

#include "resources.hpp"
#include "scene.hpp"
#include <SFML/Graphics.hpp>

class Application
{
  Scene& scene;
  sf::RenderWindow window;

public:
  Application(Scene& scene);

  int run();

};

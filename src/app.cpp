#include "app.hpp"

Application::Application(Scene& scene)
: scene(scene)
{
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  window.create(sf::VideoMode(800, 600), "entt-sfml", sf::Style::Default, settings);
  window.setVerticalSyncEnabled(true);

  scene.get_registry().ctx().emplace< sf::RenderTarget* >(&window);

  scene.get_registry().ctx().emplace< resources::WindowSize >(window.getSize());
}

int Application::run()
{
  sf::Clock clock;

  while(window.isOpen())
  {
    sf::Time elapsed = clock.restart();

    sf::Event event;
    while(window.pollEvent(event))
    {
      switch(event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::Resized:
      {
        sf::Vector2u window_size(event.size.width, event.size.height);
        scene.get_registry().ctx().get< resources::WindowSize >() = window_size;
        break;
      }
      case sf::Event::KeyPressed:
        if(event.key.code == sf::Keyboard::Escape)
        {
          window.close();
        }
        break;

      default:
        break;
      }
    }

    scene.get_registry().ctx().get< resources::DeltaTime >() = elapsed.asSeconds();
    scene.update();
    window.display();
  }

  return 0;

}

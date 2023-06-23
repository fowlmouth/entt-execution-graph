#pragma once

#include <entt/entt.hpp>

class Scene
{
  entt::registry registry;
  entt::organizer organizer;
  std::vector< entt::organizer::vertex > graph;
  std::vector< int > task_order;

public:

  Scene(entt::organizer&& );
  Scene();

  entt::organizer& get_organizer();
  entt::registry& get_registry();

  void calculate_order();
};

#pragma once

#include <entt/entt.hpp>

class Scene
{
  entt::organizer organizer;
  std::vector< entt::organizer::vertex > graph;
  std::vector< int > task_order;

public:

  entt::organizer& get_organizer();

  void calculate_order();
};

#pragma once

#include <entt/entt.hpp>

class Scene
{
  entt::registry registry;
  entt::organizer organizer;
  std::vector< entt::organizer::vertex > graph;
  std::vector< int > task_order;
  std::vector< std::pair< entt::organizer::function_type* , const void* >> tasks;

public:

  Scene(entt::organizer&& );
  Scene();

  entt::organizer& get_organizer();
  entt::registry& get_registry();
  const std::vector< entt::organizer::vertex >& get_graph() const;

  void calculate_order();
  void prepare();

  void update();
};

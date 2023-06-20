#include "scene.hpp"

entt::organizer& Scene::get_organizer()
{
  return organizer;
}

void Scene::calculate_order()
{
  graph = organizer.graph();

}

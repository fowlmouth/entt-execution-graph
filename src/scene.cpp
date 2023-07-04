#include "scene.hpp"

Scene::Scene(entt::organizer&& organizer)
: organizer(std::move(organizer))
{
}

Scene::Scene()
{
}

entt::organizer& Scene::get_organizer()
{
  return organizer;
}

entt::registry& Scene::get_registry()
{
  return registry;
}

const std::vector< entt::organizer::vertex >& Scene::get_graph() const
{
  return graph;
}

void Scene::prepare()
{
  calculate_order();
  tasks.resize(task_order.size());
  for(int i = 0; i < task_order.size(); ++i)
  {
    const auto& node = graph[task_order[i]];
    node.prepare(registry);
    tasks[i] = std::make_pair(node.callback(), node.data());
  }
}

void Scene::update()
{
  for(const auto& task : tasks)
  {
    task.first(task.second, registry);
  }
}


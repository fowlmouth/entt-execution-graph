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


#include <unordered_map>
#include <iostream>

struct typeinfo_stat
{
  std::vector< int > rw, ro;
  int rw_remaining = 0;
};


void Scene::calculate_order()
{
  graph = organizer.graph();
  std::unordered_map< const entt::type_info* , typeinfo_stat > typeinfo_stats;
  std::vector< const entt::type_info* > buffer;
  for(int i = 0; i < graph.size(); ++i)
  {
    auto rw_count = graph[i].rw_count(),
      ro_count = graph[i].ro_count();
    std::cout << "type= " << graph[i].name() << "  rw= " << rw_count << "  ro= " << ro_count << std::endl;
    buffer.resize(rw_count);
    graph[i].rw_dependency(&buffer[0], rw_count);
    for(const entt::type_info* type : buffer)
    {
      std::cout << "    rw type= " << type->name() << std::endl;
      auto& stats = typeinfo_stats[ type ];
      stats.rw.push_back(i);
      stats.rw_remaining++;
    }

    buffer.resize(ro_count);
    graph[i].ro_dependency(&buffer[0], ro_count);
    for(const entt::type_info* type : buffer)
    {
      typeinfo_stats[ type ].ro.push_back(i);
    }
  }

  std::vector< int > vertex_order;

  // find beginnings
  // types with 0 rws are potential entrypoints
  std::unordered_set< const entt::type_info* > cleared_rw_types;
  while(vertex_order.size() != graph.size())
  {
    for(const auto pair : typeinfo_stats)
    {
      if(pair.second.rw_remaining == 0)
      {
        cleared_rw_types.insert(pair.first);
      }
    }

    for(const auto type : cleared_rw_types)
    {
      // check ROs for all deps
      std::cout << "checking type= " << type->name() << std::endl;
      const auto& type_ro = typeinfo_stats[type].ro;
      for(const auto ro_vert_index : type_ro)
      {
        const auto& dep = graph[ro_vert_index];
        std::cout << "  ro vert name= " << dep.name() << std::endl;
        // if every RO type for this function is "RW cleared" then this fn can be called now
        const int ro_count = dep.ro_count();
        buffer.resize(ro_count);
        dep.ro_dependency(&buffer[0], ro_count);
        if(std::all_of(buffer.begin(), buffer.end(), [&cleared_rw_types](const entt::type_info* type){ return cleared_rw_types.contains(type); }))
        {
          // all RO deps are cleared
          std::cout << "    fully specified!" << std::endl;
        }
      }
    }

    break;
  }
}

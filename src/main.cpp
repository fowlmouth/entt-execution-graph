#include <entt/entt.hpp>
#include <iostream>
#include <cstring>

#include "systems.hpp"

auto prepare_registry(entt::organizer& organizer, entt::registry& registry)
{
  auto graph = organizer.graph();
  for(auto& node : graph)
  {
    node.prepare(registry);
  }
  return std::move(graph);
}


void debug_graph(std::ostream& out, const std::vector<entt::organizer::vertex>& graph)
{
  std::vector< const entt::type_info* > typeinfo_buffer;
  typeinfo_buffer.reserve(64);
  for(const auto& vert : graph)
  {
    auto ro_count = vert.ro_count();
    auto rw_count = vert.rw_count();

    out << "vert name= " << (vert.name() ? vert.name() : "(no name)") << std::endl;

    out << "  ro_count= " << ro_count << std::endl;
    typeinfo_buffer.resize(ro_count);
    vert.ro_dependency(& typeinfo_buffer[0], typeinfo_buffer.size());
    for(auto typeinfo : typeinfo_buffer)
    {
      out << "    " << typeinfo->name() << std::endl;
    }

    out << "  rw_count= " << rw_count << std::endl;
    typeinfo_buffer.resize(rw_count);
    vert.rw_dependency(& typeinfo_buffer[0], typeinfo_buffer.size());
    for(auto typeinfo : typeinfo_buffer)
    {
      out << "    " << typeinfo->name() << std::endl;
    }

    out
      << "  is_top_level= " << (vert.top_level() ? "yes" : "no") << std::endl
      << "  children count= " << vert.children().size() << std::endl;
    for(const size_t child : vert.children())
    {
      out << "    " << (graph[child].name() ? graph[child].name() : "(noname)") << std::endl;
    }
    out << std::endl;
  }
}

// this code is borrowed with love from github.com/MadeOfJelly/MushMachine
// graphviz dot export
static std::ostream& dot(std::ostream& out, const std::vector<entt::organizer::vertex>& nodes)
{
  out << "digraph EnTT_organizer {\nrankdir=LR;\n";

  std::unordered_set< const entt::type_info* > typeinfo_found;
  std::vector< const entt::type_info* > typeinfo_buffer;
  for(size_t i = 0; i < nodes.size(); i++)
  {
    const entt::organizer::vertex& node = nodes[i];
    out << "n" << i << "[shape=" << (node.top_level() ? "doublecircle" : "circle") << " label=\"" << (node.name() == nullptr ? "NoName" : node.name()) << "\"];\n";

    // rw nodes
    typeinfo_buffer.resize(node.rw_count());
    node.rw_dependency(& typeinfo_buffer[0], typeinfo_buffer.size());
    for(const entt::type_info* typeinfo : typeinfo_buffer)
    {
      out << "n" << i << " -> " << "t" << typeinfo->hash() << ";\n";
    }
    typeinfo_found.insert(typeinfo_buffer.begin(), typeinfo_buffer.end());

    // ro nodes
    typeinfo_buffer.resize(node.ro_count());
    node.ro_dependency(& typeinfo_buffer[0], typeinfo_buffer.size());
    for(const entt::type_info* typeinfo : typeinfo_buffer)
    {
      out << "t" << typeinfo->hash() << " -> n" << i << ";\n";
    }
    typeinfo_found.insert(typeinfo_buffer.begin(), typeinfo_buffer.end());
  }

  for(const entt::type_info* typeinfo : typeinfo_found)
  {
    out << "t" << typeinfo->hash() << " [shape=rectangle label=\"" << typeinfo->name() << "\"];\n";
  }

  return out << "}";
}

void print_graph(std::ostream& stream, entt::organizer& organizer)
{
  auto graph = organizer.graph();
  dot(stream, graph);
}



entt::entity create_falling(entt::registry& reg, float x, float y, float vy)
{
  using namespace components;

  entt::entity ent = reg.create();
  reg.emplace< Translation >(ent, x, y, 0.f);
  reg.emplace< Velocity >(ent, 0.f, vy, 0.f);
  reg.emplace< Collider >(ent);

  return ent;
}

#include "physics.hpp"
#include "app.hpp"

int main(int argc, const char** argv)
{
  scenes::PhysicsScene scene;
  auto& organizer = scene.get_organizer();
  auto& registry = scene.get_registry();
  scene.prepare();
  auto& graph = scene.get_graph();
  if(argc == 2)
  {
    if(!strcmp("--dot", argv[1]))
    {
      dot(std::cout, graph);
      return 0;
    }
    if(!strcmp("--debug", argv[1]))
    {
      debug_graph(std::cout, graph);
      std::cout << std::endl;
      return 0;
    }
    if(!strcmp("--order", argv[1]))
    {
      scene.calculate_order();
      return 0;
    }
    if(!strcmp("--test", argv[1]))
    {
      // create objects
      for(int x = 0; x < 2; ++x)
      {
        create_falling(registry, 0, 0, 1.f);
      }

      // set up time delta for frames
      auto& dt = registry.ctx().get< resources::DeltaTime >();
      dt = 1.f;

      // tick 10 times
      int ticks = 3;
      for(; ticks--; )
      {
        for(const auto& vert : graph)
        {
          const char* name = vert.name();
          std::cout << "* Start stage= " << (name ? name : "(noname)") << std::endl;
          vert.callback()(vert.data(), registry);
        }

        auto view = registry.view< components::Translation >();
        for(auto ent : view)
        {
          const auto& t = view.get< components::Translation >(ent);
          std::cout << "ent id= " << (int)ent << "  x= " << t.x << " y= " << t.y << std::endl;
        }
        std::cout << std::endl;
      }
      return 0;
    }
  }

  {
    entt::entity ship = registry.create();

    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;

    b2Body *body = registry.ctx().get< resources::PhysicsWorld >()->CreateBody(&body_def);
    registry.emplace< components::Body >(ship, body);

    b2FixtureDef fixture_def;

    b2CircleShape shape;
    shape.m_radius = 1.f;
    fixture_def.shape = &shape;

    fixture_def.density = 1.f;
    fixture_def.friction = 0.3f;

    b2Fixture *fixture = body->CreateFixture(&fixture_def);
    registry.emplace< components::Fixture >(ship, fixture);

  }

  Application app(scene);
  return app.run();
}

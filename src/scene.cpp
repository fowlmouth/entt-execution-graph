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


#include <iostream>
#include <algorithm>

// temporary node graph structure for working with the entt::organizer vertices

struct node;

struct node_graph
{
  const std::vector< entt::organizer::vertex >& graph;
  std::vector< node* > nodes;

  node_graph(const std::vector< entt::organizer::vertex >& graph);
  ~node_graph();

  std::vector< node* > entrypoints() const;

  std::vector< int > calculate_order() const;

private:
  node* insert(const int function_vertex);
  node* insert(const entt::type_info* type);
};

struct node
{
  enum{ type, function } kind;
  union
  {
    const entt::type_info* type_;
    int vertex_;
  };
  std::string_view name;

  std::vector< node* > in, out;
};

node_graph::node_graph(const std::vector< entt::organizer::vertex >& graph)
: graph(graph)
{
  for(int i = 0; i < graph.size(); ++i)
  {
    insert(i);
  }
}

node_graph::~node_graph()
{
  for(node* n : nodes)
  {
    delete n;
  }
}

std::vector< node* > node_graph::entrypoints() const
{
  std::vector< node* > result;
  std::copy_if(nodes.begin(), nodes.end(),
    std::back_inserter(result),
    [](node* n){ return n->in.size() == 0; });
  return std::move(result);
}

std::vector< int > node_graph::calculate_order() const
{
  // 1. starting with the entrypoints, mark off nodes as completed
  // 2. scan the list of remaining nodes, nodes with all "in" parameters marked as completed can be marked as completed.
  //      if the node is a function, add its index to result vector
  // 3. repeat step 2 until no nodes remain

  std::vector< int > result;

  std::unordered_set< node* > cleared;
  std::vector< node* > remaining = nodes;
  while(!remaining.empty())
  {
    for(int i = 0; i < remaining.size(); )
    {
      node* n = remaining[i];
      if(std::all_of(n->in.begin(), n->in.end(), [&](node* x){ return cleared.contains(x); }))
      {
        cleared.insert(n);
        if(n->kind == node::function)
        {
          result.push_back(n->vertex_);
        }
        if(i < remaining.size()-1)
        {
          remaining[i] = remaining.back();
        }
        remaining.pop_back();
        continue;
      }
      ++i;
    }
  }

  return result;
}

node* node_graph::insert(const int function_vertex)
{
  auto iter = std::find_if(nodes.begin(), nodes.end(), [function_vertex](node* n){ return n->kind == node::function && n->vertex_ == function_vertex; });
  if(iter == nodes.end())
  {
    const auto& vertex = graph[function_vertex];

    node* n = new node;
    n->kind = node::function;
    n->vertex_ = function_vertex;
    n->name = vertex.name();
    nodes.push_back(n);

    std::vector< const entt::type_info* > buffer;
    buffer.resize(vertex.ro_count());
    vertex.ro_dependency(&buffer[0], buffer.size());
    for(const entt::type_info* type : buffer)
    {
      node* type_node = insert(type);
      n->in.push_back(type_node);
      type_node->out.push_back(n);
    }

    buffer.resize(vertex.rw_count());
    vertex.rw_dependency(&buffer[0], buffer.size());
    for(const entt::type_info* type : buffer)
    {
      node* type_node = insert(type);
      n->out.push_back(type_node);
      type_node->in.push_back(n);
    }

    return n;
  }
  return *iter;
}

node* node_graph::insert(const entt::type_info* type)
{
  auto iter = std::find_if(nodes.begin(), nodes.end(), [&type](node* n){ return n->kind == node::type && n->type_ == type; });
  if(iter == nodes.end())
  {
    node* n = new node;
    n->kind = node::type;
    n->type_ = type;
    n->name = type->name();
    nodes.push_back(n);
    return n;
  }
  return *iter;
}


void Scene::calculate_order()
{
  graph = organizer.graph();
  node_graph ng(graph);
  std::vector< node* > entrypoints = ng.entrypoints();
  std::cout << "entrypoints:" << std::endl;
  for(node* n : entrypoints)
  {
    std::cout << "  name= " << n->name << "  type= " << (n->kind == node::type ? "type" : "function") << std::endl;
  }
  auto order = ng.calculate_order();
  std::cout << "order calculated:" << std::endl;
  for(int idx : order)
  {
    std::cout << "  name= " << graph[idx].name() << std::endl;
  }

}

#pragma once

#include <box2d/box2d.h>

#include "components.hpp"
#include "resources.hpp"
#include "scene.hpp"
#include "systems.hpp"

namespace components
{

struct Body
{
  b2Body* body;
};

struct Fixture
{
  b2Fixture* fixture;
};


} // ::components



namespace resources
{

struct PhysicsWorld
{
  b2World world;
  int velocity_iterations = 6;
  int position_iterations = 2;

  PhysicsWorld();

  inline operator b2World&()
  {
    return world;
  }

  inline operator const b2World&() const
  {
    return world;
  }

  inline b2World* operator->()
  {
    return &world;
  }
};

struct PhysicsDebugRenderEnabled : public BasicResource< bool >
{
};

} // ::resources



namespace systems
{

void step_world(
  entt::view< entt::get_t< const components::Body >> dummy,
  resources::PhysicsWorld& world,
  const resources::DeltaTime& dt);

void physics_interpret_inputs(
  entt::view< entt::get_t< components::Body, const components::InputState >> input_objects,
  const resources::DeltaTime& dt);

void physics_debug_render(
  const resources::PhysicsDebugRenderEnabled& enabled,
  const resources::PhysicsWorld& world,
  const resources::Viewport& viewport,
  resources::RenderGroup& render_group);

} // ::systems



namespace scenes
{

class PhysicsScene : public ::Scene
{
public:
  PhysicsScene();
};

} // ::scenes


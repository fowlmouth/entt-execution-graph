#include "physics.hpp"

namespace resources
{

PhysicsWorld::PhysicsWorld()
: world(b2Vec2(0.0f, -9.8f))
{
}

} // ::resources



namespace systems
{

void step_world(
  entt::view< entt::get_t< const components::Body >> dummy,
  resources::PhysicsWorld& world,
  const resources::DeltaTime& dt)
{
  world->Step(dt, world.velocity_iterations, world.position_iterations);
}

void physics_interpret_inputs(
  entt::view< entt::get_t< components::Body, const components::InputState >> input_objects,
  const resources::DeltaTime& dt)
{
  for(auto&& [ent, body, input] : input_objects.each())
  {
    // apply forces to body
  }
}

void physics_debug_render(
  const resources::PhysicsDebugRenderEnabled& enabled,
  const resources::PhysicsWorld& world,
  const resources::Viewport& viewport,
  resources::RenderGroup& render_group)
{
  if(!enabled)
    return;

  // query world

  // render to drawable

}

} // ::systems

namespace scenes
{

PhysicsScene::PhysicsScene()
: Scene(systems::organizer())
{
  get_organizer().emplace< &systems::step_world >("step_world");
  get_organizer().emplace< &systems::physics_interpret_inputs >("physics_interpret_inputs");
  get_organizer().emplace< &systems::physics_debug_render >("physics_debug_render");
}

} // ::scenes

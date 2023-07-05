#pragma once

#include "components.hpp"
#include "resources.hpp"

#include <entt/entt.hpp>

#include <SFML/Graphics.hpp>

namespace systems
{
  using namespace components;
  using namespace resources;

  void apply_velocity(entt::view< entt::get_t< Translation , const Velocity >> moving_objects, const DeltaTime& dt);

  void interpret_inputs(entt::view< entt::get_t< Velocity , const InputState >> input_velocity_entities);

  void read_devices(entt::view< entt::get_t< InputState, const Controller >> controllable_entities, const DeviceInterface& hid);

  void read_computer_inputs(entt::view< entt::get_t< InputState, const ComputerController >> computer_controllers);

  void cache_boundingbox(entt::registry& registry, entt::view< entt::get_t< const Translation, const BoundingBoxProvider, BoundingBox >> bb_objects);

  void check_collisions(entt::view< entt::get_t< const Collider, const BoundingBox >>);

  void update_camera(entt::registry& , entt::view< entt::get_t< const Translation >>, Camera& );

  void update_viewport(entt::registry& , const Camera& , const WindowSize& , Viewport& );

  void query_world(entt::registry& , entt::view< entt::get_t< const BoundingBox, const DrawableProvider >>, const Viewport&, RenderGroup& );

  void render_world(entt::registry& , const Viewport& , const RenderGroup& , sf::RenderTarget*& );

  entt::organizer organizer();

} // ::systems


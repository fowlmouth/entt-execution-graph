#include "systems.hpp"

namespace systems
{

void apply_velocity(entt::view< entt::get_t< Translation , const Velocity >> moving_objects, const DeltaTime& dt)
{
  for(auto ent : moving_objects)
  {
    auto& translation = moving_objects.get<Translation>(ent);
    const auto& velocity = moving_objects.get<Velocity>(ent);
    translation.x += velocity.x * dt; //.delta_time_seconds;
    translation.y += velocity.y * dt; //.delta_time_seconds;
    translation.radians += velocity.angular * dt; //.delta_time_seconds;
  }
}

void interpret_inputs(entt::view< entt::get_t< Velocity , const InputState >> input_velocity_entities)
{
}

void read_devices(entt::view< entt::get_t< InputState, const Controller >> controllable_entities, const DeviceInterface& hid)
{
  for(auto ent : controllable_entities)
  {
    InputState& input_state = controllable_entities.get< InputState >(ent);
    const Controller& ctrl = controllable_entities.get< Controller >(ent);
    hid.check_controls(ctrl.controls, &input_state);
  }
}

void read_computer_inputs(entt::view< entt::get_t< InputState, const ComputerController >> computer_controllers)
{
  (void)0;
}

void cache_boundingbox(entt::registry& registry, entt::view< entt::get_t< const Translation, const BoundingBoxProvider, BoundingBox >> bb_objects)
{
  for(auto ent : bb_objects)
  {
    const auto& translation = bb_objects.get< Translation >(ent);
    const auto& provider = bb_objects.get< BoundingBoxProvider >(ent);
    registry.replace< BoundingBox >(ent, provider(registry, ent));
  }
}

void check_collisions(entt::view< entt::get_t< const Collider, const BoundingBox >>)
{

}

void update_camera(entt::registry& registry, entt::view< entt::get_t< const Translation >> view, Camera& camera)
{
  if(camera.tracking != entt::null)
  {
    if(!registry.valid(camera.tracking))
    {
      camera.tracking = entt::null;
      return;
    }
    const auto& translation = view.get< Translation >(camera.tracking);
    camera.x = translation.x;
    camera.y = translation.y;
  }
}

void update_viewport(entt::registry& , const Camera& camera, const WindowSize& window_size, Viewport& viewport)
{
  float w = camera.scale_x * (float)window_size.x;
  float h = camera.scale_y * (float)window_size.y;
  viewport.left = camera.x - w / 2;
  viewport.top  = camera.y - h / 2;
  viewport.width  = w;
  viewport.height = h;
}

void query_world(entt::registry& , entt::view< entt::get_t< const BoundingBox, const DrawableProvider >>, const Viewport&, RenderGroup& )
{
}

void render_world(entt::registry& , const Viewport& , const RenderGroup& , sf::RenderTarget*& target )
{
}

entt::organizer organizer()
{
  entt::organizer organizer;
  organizer.emplace< &systems::apply_velocity >("apply_velocity");
  organizer.emplace< &systems::interpret_inputs >("interpret_inputs");
  organizer.emplace< &systems::read_devices >("read_devices");
  organizer.emplace< &systems::read_computer_inputs >("read_computer_inputs");
  organizer.emplace< &systems::cache_boundingbox >("cache_boundingbox");
  organizer.emplace< &systems::check_collisions >("check_collisions");
  organizer.emplace< &systems::update_camera >("update_camera");
  organizer.emplace< &systems::update_viewport >("update_viewport");
  organizer.emplace< &systems::query_world >("query_world");
  organizer.emplace< &systems::render_world >("render_world");
  return std::move(organizer);
}

} // ::systems

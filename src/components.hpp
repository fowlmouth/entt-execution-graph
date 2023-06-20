#pragma once

#include <entt/entt.hpp>

#include "hid.hpp"

namespace components
{

struct Translation
{
  float x, y, radians;
};

struct Velocity
{
  float x, y, angular;
};

struct Mesh
{
};

struct Material
{
};

struct InputState
{
  float move_x, move_y;
};

struct Collider
{
};

struct Controller
{
  hid::ControllerDefinition* controls;
};

struct ComputerController
{
};

struct BoundingBox
{
  float left, top, width, height;
};

struct Drawable
{
};


template< typename ResultT >
struct Provider
{
  using Fn = ResultT(*)(entt::registry& , entt::entity);

  Provider(Fn fn)
  : fn(fn)
  {
  }

  inline ResultT operator()(entt::registry& reg, entt::entity ent) const
  {
    return fn(reg, ent);
  }

  inline operator bool() const
  {
    return fn != nullptr;
  }

protected:
  Fn fn;
};

using BoundingBoxProvider = Provider< BoundingBox >;
using DrawableProvider = Provider< Drawable >;

} // ::components

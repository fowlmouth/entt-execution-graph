#pragma once

#include <entt/entt.hpp>

#include "hid.hpp"

namespace resources
{


using DeviceInterface = hid::DeviceInterface;

struct DeltaTime
{
  float delta_time_seconds;
};

struct Camera
{
  float x, y;
  entt::entity tracking;
  float viewport_width, viewport_height;
};

struct Viewport
{
  float left, top, width, height;
};

struct RenderGroup
{
  struct Drawable
  {
    int zorder;
  };
  std::vector< Drawable > drawable;
};


} // ::resources

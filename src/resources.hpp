#pragma once

#include <entt/entt.hpp>

#include "hid.hpp"

namespace resources
{


using DeviceInterface = hid::DeviceInterface;



template< typename T >
struct BasicResource
{
  T value;

  BasicResource()
  {
  }

  BasicResource(const T& value)
  : value(value)
  {
  }

  BasicResource(T&& other)
  : value(std::move(other))
  {
  }

  BasicResource& operator=(const T& new_value)
  {
    value = new_value;
    return *this;
  }

  BasicResource& operator=(T&& other)
  {
    value = std::move(other);
    return *this;
  }

  inline operator const T&() const
  {
    return value;
  }

  inline operator T&()
  {
    return value;
  }
};

struct DeltaTime : public BasicResource< float >
{
  using Parent = BasicResource<float>;

  using Parent::operator=;
};

struct Camera
{
  float x, y;
  entt::entity tracking = entt::null;
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

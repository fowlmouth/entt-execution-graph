#pragma once

#include <entt/entt.hpp>

#include "hid.hpp"

#include <SFML/System.hpp>

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

  using Parent::operator= ;
};



struct Camera
{
  float x = 0, y = 0;
  entt::entity tracking = entt::null;
  float scale_x = 1.f, scale_y = 1.f;
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



struct WindowSize : public sf::Vector2u
{
  using Parent = sf::Vector2u ;

  using Parent::operator= ;
};




} // ::resources
